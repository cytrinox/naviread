/*
    Copyright (C) 2008 Alexander Steffen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdlib.h>
#include <stdio.h>
#include "navilib.h"


int main(int argc, char *argv[], char *env[])
{
  FILE *nvpipe;
  int trackcount;
  struct trackpoint point;
  
  if (argc != 2)
  {
    printf("Usage: %s FILE\n", argv[0]);
    return 0;
  }
  
  nvpipe = fopen(argv[1], "rb");
  
  if (nvpipe == NULL)
  {
    printf("error opening file '%s'\n", argv[1]);
    return 1;
  }
  
  // Datensätze zu 16 Byte beginnen ab Adresse 0x00001000
  fseek(nvpipe, 0x00001000, SEEK_SET);
  trackcount = 0;
  
  puts("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
  puts("<gpx version=\"1.1\" creator=\"naviread 0.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/1\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">");
  
  while (read_point(nvpipe, &point) == 0)
  {
    if (point.type == 1)
    {
      // ggf. alten Track abschließen
      if (trackcount > 0)
      {
        puts("</trkseg>");
        puts("</trk>");
      }

      // neuen Track starten
      puts("<trk>");
      printf("<name>Track %d</name>\n", trackcount);
      printf("<number>%d</number>\n", trackcount+1);
      puts("<trkseg>");
      trackcount++;
    }
    
    // Trackpunkt ausgeben
    printf("<trkpt lat=\"%f\" lon=\"%f\">\n", (double)point.latitude/10000000, (double)point.longitude/10000000);
    printf("<ele>%f</ele>\n", (double)point.height);
    printf("<time>%s</time>\n", navitime_to_str(point.time));
    puts("</trkpt>");
  }

  fclose(nvpipe);
  
  // ggf. alten Track abschließen
  if (trackcount > 0)
  {
    puts("</trkseg>");
    puts("</trk>");
  }

  puts("</gpx>");
  
  return 0;
}
