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

//! \file configuration.c
//! \brief Configuration-specific functions

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "navi.h"
#include "configuration.h"

//! \brief Encode password string for storage in logger configuration
//!
//! Replaces all unused characters up to the maximum of ten characters with PASSWORD_EMPTY.
//! \param password Password as string
void configuration_password_encode(char *password)
{
	memset(password+strlen(password), PASSWORD_EMPTY, 10-strlen(password));
}

//! \brief Decode password string from storage in logger configuration
//!
//! Null-terminates the password so it can be used as a C string.
//! \param password Password in encoded form
void configuration_password_decode(char *password)
{
	int i = 0;
	unsigned char *ptr = (unsigned char*)password;
	while (*(ptr++) != PASSWORD_EMPTY && i++ < 10);
	*(ptr-1) = 0;
}

//! \brief Read single configuration setting
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \param offset Offset of setting location from beginning of file
//! \param size Storage size of setting
//! \param buffer Location to store data
//! \return result.RESULT_ERROR on read error\n
//! result.RESULT_OK otherwise
enum result configuration_read_setting(FILE *nvpipe, enum nvpipe_offset offset, int size, void *buffer)
{
	if (fseek(nvpipe, offset, SEEK_SET) != 0) return RESULT_ERROR;
	if (fread(buffer, size, 1, nvpipe) != 1) return RESULT_ERROR;
	return RESULT_OK;
}

//! \brief Write single configuration setting
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \param offset Offset of setting location from beginning of file
//! \param size Storage size of setting
//! \param buffer Location to read data from
//! \return result.RESULT_ERROR on write error\n
//! result.RESULT_OK otherwise
enum result configuration_write_setting(FILE *nvpipe, enum nvpipe_offset offset, int size, void *buffer)
{
	if (fseek(nvpipe, offset, SEEK_SET) != 0) return RESULT_ERROR;
	if (fwrite(buffer, size, 1, nvpipe) != 1) return RESULT_ERROR;
	return RESULT_OK;
}

//! \brief Read/write complete configuration
//!
//! \param nvpipe File handle of NVPIPE.DAT
//! \param nvconf Storage location of configuration data
//! \param access ACCESS_WRITE to write data to NVPIPE.DAT\n
//! ACCESS_READ to read data from NVPIPE.DAT
//! \return result.RESULT_ERROR on write error\n
//! result.RESULT_OK otherwise
enum result configuration_readwrite(FILE *nvpipe, struct naviconf *nvconf, enum file_access access)
{
	// select access function
	enum result (*ptr)(FILE *, enum nvpipe_offset, int, void *);

	if (access == ACCESS_WRITE)
	{
		ptr = &configuration_write_setting;
	}
	else
	{
		ptr = &configuration_read_setting;
	}

	// read or write values (depending on access function)
	if (ptr(nvpipe, OFFSET_LOG_MODE, 1, &nvconf->log_mode) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_CLEAR_MEMORY, 1, &nvconf->clear_memory) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_DEVICE_ZONE, 2, &nvconf->device_zone) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_SHAKE_MODE, 1, &nvconf->shake_mode) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_SHAKE_MODE_TIME, 1, &nvconf->shake_mode_time) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_LOG_MODE, 1, &nvconf->userdefined_log_mode) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_CONTRAST, 1, &nvconf->contrast) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_SYSTEM_UNIT, 1, &nvconf->system_unit) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_BACKLIGHT_TIME, 1, &nvconf->backlight_time) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_TIME_INTERVAL, 2, &nvconf->userdefined_time_interval) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_DISTANCE_INTERVAL, 2, &nvconf->userdefined_distance_interval) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_HEADING_CHANGE, 1, &nvconf->userdefined_heading_change) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_HIGHEST_SPEED, 2, &nvconf->userdefined_highest_speed) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_HIGH_SPEED, 2, &nvconf->userdefined_high_speed) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_MIDDLE_SPEED, 2, &nvconf->userdefined_middle_speed) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_LOW_SPEED, 2, &nvconf->userdefined_low_speed) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_LOWEST_SPEED, 2, &nvconf->userdefined_lowest_speed) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_HIGHEST_SPEED_INTERVAL, 2, &nvconf->userdefined_highest_speed_interval) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_HIGH_SPEED_INTERVAL, 2, &nvconf->userdefined_high_speed_interval) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_MIDDLE_SPEED_INTERVAL, 2, &nvconf->userdefined_middle_speed_interval) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_USERDEFINED_LOW_SPEED_INTERVAL, 2, &nvconf->userdefined_low_speed_interval) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_PASSWORD, 10, &nvconf->password) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_INITIAL_MIN_SVS, 1, &nvconf->initial_min_svs) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_FIX_MODE, 1, &nvconf->fix_mode) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_FIX_ALTITUDE, 3, &nvconf->fix_altitude) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_INITIAL_MIN_STRENGTH, 1, &nvconf->initial_min_strength) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_NAVIGATION_MIN_STRENGTH, 1, &nvconf->navigation_min_strength) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_PDOP_MASK, 2, &nvconf->pdop_mask) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_TDOP_MASK, 2, &nvconf->tdop_mask) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_P_ACCURACY_MASK, 2, &nvconf->p_accuracy_mask) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_T_ACCURACY_MASK, 2, &nvconf->t_accuracy_mask) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_CHECKSUM, 2, &nvconf->checksum) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_SBAS, 2, &nvconf->sbas) == RESULT_ERROR) return RESULT_ERROR;
	if (ptr(nvpipe, OFFSET_GPS_MODE, 2, &nvconf->gps_mode) == RESULT_ERROR) return RESULT_ERROR;

	return RESULT_OK;
}

//! \brief Read complete configuration
//!
//! Decodes password so it can be used as a C string.
//! \param nvpipe File handle of NVPIPE.DAT
//! \param nvconf Storage location of configuration data
//! \return result.RESULT_ERROR on write error\n
//! result.RESULT_OK otherwise
enum result configuration_read(FILE *nvpipe, struct naviconf *nvconf)
{
	if (configuration_readwrite(nvpipe, nvconf, ACCESS_READ) == RESULT_ERROR) return RESULT_ERROR;
	configuration_password_decode(nvconf->password);

	return RESULT_OK;
}

//! \brief Write complete configuration
//!
//! Takes care encoding of password before writing and decoding afterwards.
//! \param nvpipe File handle of NVPIPE.DAT
//! \param nvconf Storage location of configuration data
//! \return result.RESULT_ERROR on write error\n
//! result.RESULT_OK otherwise
enum result configuration_write(FILE *nvpipe, struct naviconf *nvconf)
{
	configuration_password_encode(nvconf->password);
	if (configuration_readwrite(nvpipe, nvconf, ACCESS_WRITE) == RESULT_ERROR)
	{
		configuration_password_decode(nvconf->password);
		return RESULT_ERROR;
	}
	configuration_password_decode(nvconf->password);

	return RESULT_OK;
}

//! \brief Print configuration settings
//!
//! \param nvconf Storage location of configuration data
void configuration_print(struct naviconf *nvconf)
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
