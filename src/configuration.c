/*
	Copyright (C) 2008-2010 Alexander Steffen

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

#include <string.h>
#include <stdlib.h>
#include "navi.h"
#include "configuration.h"


int read_conf(FILE *nvpipe, struct naviconf *nvconf)
{
	if (fseek(nvpipe, 0x0000, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0002, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->clear_memory, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0004, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->device_zone, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0006, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->shake_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0008, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->shake_mode_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000C, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->contrast, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->system_unit, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000F, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->backlight_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0010, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_time_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0012, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_distance_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0014, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_heading_change, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0016, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_highest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0018, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_high_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_middle_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001C, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_low_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_lowest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0020, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_highest_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0022, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_high_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0024, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_middle_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0026, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->userdefined_low_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0204, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->password, 10, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->initial_min_svs, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036D, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->fix_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->fix_altitude, 3, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0372, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->initial_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0373, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->navigation_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037A, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->pdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037C, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->tdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->p_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0380, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->t_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x038E, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->checksum, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0396, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->sbas, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x03FE, SEEK_SET) != 0) return -1;
	if (fread(&nvconf->gps_mode, 2, 1, nvpipe) != 1) return -1;

	int i = 0;
	unsigned char *ptr = (unsigned char*)nvconf->password;
	while (*(ptr++) != 0xEE && i++ < 10);
	*(ptr-1) = 0;

	return 0;
}

int write_conf(FILE *nvpipe, struct naviconf *nvconf)
{
	memset(nvconf->password+strlen(nvconf->password), 0xEE, 10-strlen(nvconf->password));

	if (fseek(nvpipe, 0x0000, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0002, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->clear_memory, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0004, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->device_zone, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0006, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->shake_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0008, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->shake_mode_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_log_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000C, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->contrast, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->system_unit, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x000F, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->backlight_time, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0010, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_time_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0012, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_distance_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0014, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_heading_change, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0016, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_highest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0018, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_high_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_middle_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001C, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_low_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x001E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_lowest_speed, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0020, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_highest_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0022, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_high_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0024, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_middle_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0026, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->userdefined_low_speed_interval, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0204, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->password, 10, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->initial_min_svs, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036D, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->fix_mode, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x036E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->fix_altitude, 3, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0372, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->initial_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0373, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->navigation_min_strength, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037A, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->pdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037C, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->tdop_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x037E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->p_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0380, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->t_accuracy_mask, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x038E, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->checksum, 2, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x0396, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->sbas, 1, 1, nvpipe) != 1) return -1;
	if (fseek(nvpipe, 0x03FE, SEEK_SET) != 0) return -1;
	if (fwrite(&nvconf->gps_mode, 1, 1, nvpipe) != 1) return -1;

	int i = 0;
	unsigned char *ptr = (unsigned char*)nvconf->password;
	while (*(ptr++) != 0xEE && i++ < 10);
	*(ptr-1) = 0;

	return 0;
}

void print_conf(struct naviconf *nvconf)
{
	puts("Geräteeinstellungen");
	puts("-------------------");

	printf("Maßeinheiten: ");
	switch (nvconf->system_unit)
	{
		case SYSTEM_UNIT_METRIC: printf("metrisches System\n"); break;
		case SYSTEM_UNIT_IMPERIAL: printf("imperiales System\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("Zeitzone: UTC%+03d:%02d\n", nvconf->device_zone/100, (int)((abs(nvconf->device_zone)%100)*60/100));
	printf("Passwort: %s\n", nvconf->password);
	printf("Kontrast: %d\n", nvconf->contrast);
	printf("Hintergrundbeleuchtung: %ds\n", nvconf->backlight_time);

	printf("Shake-Modus: ");
	switch (nvconf->shake_mode)
	{
		case SHAKE_MODE_OFF: printf("aus\n"); break;
		case SHAKE_MODE_ON: printf("an\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("Shake-Modus-Zeit: %dm\n", nvconf->shake_mode_time);

	printf("Speicher leeren: ");
	switch (nvconf->clear_memory)
	{
		case CLEAR_MEMORY_NO: printf("nein\n"); break;
		case CLEAR_MEMORY_YES: printf("ja\n"); break;
		default: printf("unbekannt\n"); break;
	}


	puts("");
	puts("Logeinstellungen");
	puts("----------------");

	printf("Log-Modus: ");
	switch (nvconf->log_mode)
	{
		case LOG_MODE_WALK: printf("Fußgänger\n"); break;
		case LOG_MODE_BICYCLE: printf("Fahrrad\n"); break;
		case LOG_MODE_CAR: printf("Auto\n"); break;
		case LOG_MODE_USER: printf("benutzerdefiniert\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("benutzerdef. Log-Modus: ");
	switch (nvconf->userdefined_log_mode)
	{
		case USERDEFINED_LOG_MODE_TIME: printf("Zeitintervall\n"); break;
		case USERDEFINED_LOG_MODE_DISTANCE: printf("Entfernungsintervall\n"); break;
		case USERDEFINED_LOG_MODE_HEADING: printf("Richtungswechsel\n"); break;
		case USERDEFINED_LOG_MODE_SPEED: printf("Geschwindigkeitswechsel\n"); break;
		case USERDEFINED_LOG_MODE_MIXED: printf("gemischt (Zeit und Entfernung)\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("Zeitintervall: %ds\n", nvconf->userdefined_time_interval);
	printf("Entfernungsintervall: %dm\n", nvconf->userdefined_distance_interval);
	printf("Richtungswechsel: %d°\n", nvconf->userdefined_heading_change);
	printf("höchste Geschwindigkeit: %dkm/h\n", nvconf->userdefined_highest_speed);
	printf("hohe Geschwindigkeit: %dkm/h\n", nvconf->userdefined_high_speed);
	printf("mittlere Geschwindigkeit: %dkm/h\n", nvconf->userdefined_middle_speed);
	printf("niedrige Geschwindigkeit: %dkm/h\n", nvconf->userdefined_low_speed);
	printf("niedrigste Geschwindigkeit: %dkm/h\n", nvconf->userdefined_lowest_speed);
	printf("Logintervall hohe-höchste Geschwindigkeit: %ds\n", nvconf->userdefined_highest_speed_interval);
	printf("Logintervall mittlere-hohe Geschwindigkeit: %ds\n", nvconf->userdefined_high_speed_interval);
	printf("Logintervall niedrige-mittlere Geschwindigkeit: %ds\n", nvconf->userdefined_middle_speed_interval);
	printf("Logintervall niedrigste-niedrige Geschwindigkeit: %ds\n", nvconf->userdefined_low_speed_interval);


	puts("");
	puts("GPS-Einstellungen");
	puts("-----------------");

	printf("GPS-Modus: ");
	switch (nvconf->gps_mode)
	{
		case GPS_MODE_HIGH_PRECISION: printf("hohe Genauigkeit\n"); break;
		case GPS_MODE_MIDDLE_PRECISION: printf("mittlere Genauigkeit\n"); break;
		case GPS_MODE_DEFAULT: printf("Werkseinstellung\n"); break;
		case GPS_MODE_MIDDLE_TIME: printf("mittlere Verbindungszeit\n"); break;
		case GPS_MODE_FAST_TIME: printf("schnelle Verbindungszeit\n"); break;
		case GPS_MODE_USER: printf("benutzerdefiniert\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("SBAS: ");
	switch (nvconf->sbas)
	{
		case SBAS_OFF: printf("aus\n"); break;
		case SBAS_ON: printf("an\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("Fix-Modus: ");
	switch (nvconf->fix_mode)
	{
		case FIX_MODE_2D: printf("nur 2D\n"); break;
		case FIX_MODE_2D3D: printf("2D/3D-Automatik\n"); break;
		case FIX_MODE_3D: printf("nur 3D\n"); break;
		default: printf("unbekannt\n"); break;
	}

	printf("2D-Höhe: %dcm\n", nvconf->fix_altitude);
	printf("initiale minimale SVs: %d\n", nvconf->initial_min_svs);
	printf("initiale Signalmindeststärke: %ddBHz\n", nvconf->initial_min_strength);
	printf("Navigations-Signalmindeststärke: %ddBHz\n", nvconf->navigation_min_strength);
	printf("P-Accuracy-Mask: %dm\n", nvconf->p_accuracy_mask);
	printf("T-Accuracy-Mask: %dm\n", nvconf->t_accuracy_mask);
	printf("PDOP-Mask: %.1f\n", (double)nvconf->pdop_mask/10);
	printf("TDOP-Mask: %.1f\n", (double)nvconf->tdop_mask/10);
	printf("Checksumme?: %d\n", nvconf->checksum);
}
