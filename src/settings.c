#include <stdlib.h>
#include <glib.h>
#include <locale.h>
#include <string.h>
#include "debug.h"
#include "settings.h"

struct ParsePack_s {
	const char *name;
	const char **aName;
	const char **aVol;
};

typedef void (*ParsFn)(const struct ParsePack_s *vol);

struct LoadVars_s {
	const char *name;
	ParsFn pars;
};


static const char *_speedToString[] = {"None", "GPS", "Encoder", "Fixing7"};

static void start_element(GMarkupParseContext *ctx, const char *eName, const char **attrName, 
		const char **attrValue, gpointer pData, GError **err);

static void end_element(GMarkupParseContext *ctx, const char *eName, gpointer pData, GError **err);

static void text_element(GMarkupParseContext *ctx, const char *txt, gsize len, gpointer pData, GError **err);
static void pass_element(GMarkupParseContext *ctx, const char *txt, gsize len, gpointer pData, GError **err);
static void err_element(GMarkupParseContext *ctx, GError *err, gpointer pData);
//--------------------------------------------------------------------------
static void array_set_int(int *arr, uint16_t len, const char *data);
static void array_set_double(double *arr, uint16_t len, const char *data);
static const char *get_vol(const struct ParsePack_s *pp, const char *aName);

static const GMarkupParser parserOptions = {start_element, end_element, text_element, pass_element, err_element};

static void agro_mode(const struct ParsePack_s *vol);
static void agro_demo(const struct ParsePack_s *vol);
static void bunkers_count(const struct ParsePack_s *vol);
static void sections_count(const struct ParsePack_s *vol);
static void towers_count(const struct ParsePack_s *vol);
static void semens_count(const struct ParsePack_s *vol);
static void crops_type(const struct ParsePack_s *vol);
static void fans_count(const struct ParsePack_s *vol);
static void fan_impuls_count(const struct ParsePack_s *vol);
static void vals_count(const struct ParsePack_s *vol);
static void mod_crops_count(const struct ParsePack_s *vol);
static void mod_manure_count(const struct ParsePack_s *vol);
static void sensor_crops_count(const struct ParsePack_s *vol);
static void sensor_manure_count(const struct ParsePack_s *vol);
static void control_level_crops(const struct ParsePack_s *vol);
static void control_level_manure(const struct ParsePack_s *vol);
static void sensor_count_tv2(const struct ParsePack_s *vol);
static void is_encoder_tv2(const struct ParsePack_s *vol);
static void is_fan1_tv2(const struct ParsePack_s *vol);
static void is_fan2_tv2(const struct ParsePack_s *vol);
static void imp2cycle_fan_tv2(const struct ParsePack_s *vol);
static void imp2cycle_fan2_tv2(const struct ParsePack_s *vol);
static void is_press_tv2(const struct ParsePack_s *vol);
static void is_press2_tv2(const struct ParsePack_s *vol);
static void is_actuators_tv2(const struct ParsePack_s *vol);
static void calc_double_tv2(const struct ParsePack_s *vol);
static void streem_toms_tv2(const struct ParsePack_s *vol);
static void is_sow_width(const struct ParsePack_s *vol);
static void sow_width(const struct ParsePack_s *vol);
static void left_width(const struct ParsePack_s *vol);
static void right_width(const struct ParsePack_s *vol);
static void sow_over(const struct ParsePack_s *vol);
static void gisto_show(const struct ParsePack_s *vol);
static void gisto_yellow_exist(const struct ParsePack_s *vol);
static void gisto_yellow_procent(const struct ParsePack_s *vol);
static void gisto_red_exist(const struct ParsePack_s *vol);
static void gisto_red_procent(const struct ParsePack_s *vol);
static void plug_down_inversion(const struct ParsePack_s *vol);
static void plug_down_force(const struct ParsePack_s *vol);
static void depth_sensors_exists(const struct ParsePack_s *vol);
static void depth_sensors_maskaon(const struct ParsePack_s *vol);
static void fix_change_stream(const struct ParsePack_s *vol);
static void sensors_combine_count(const struct ParsePack_s *vol);
static void scrapers_count(const struct ParsePack_s *vol);
static void sensor_combine_adr(const struct ParsePack_s *vol);
static void scraper_width(const struct ParsePack_s *vol);
static void scraper_depth(const struct ParsePack_s *vol);
static void scraper_dist(const struct ParsePack_s *vol);
static void combine_sum_vol(const struct ParsePack_s *vol);
static void section_exists(const struct ParsePack_s *vol);
static void speed_method(const struct ParsePack_s *vol);
static void speed_encoder_constant(const struct ParsePack_s *vol);
static void speed_encoder_imp2metr(const struct ParsePack_s *vol);
static void encoder_calibrate_control(const struct ParsePack_s *vol);
static void procent_alarm_calibrate_encoder(const struct ParsePack_s *vol);
static void fan_gidro(const struct ParsePack_s *vol);
static void fan_dvigatel(const struct ParsePack_s *vol);
static void time_sync(const struct ParsePack_s *vol);
static void time_region(const struct ParsePack_s *vol);
static void port_contr(const struct ParsePack_s *vol);
static void port_control(const struct ParsePack_s *vol);
static void port_speed(const struct ParsePack_s *vol);
static void pasw_common(const struct ParsePack_s *vol);
static void nop_func(const struct ParsePack_s *vol);

static struct LoadVars_s lv[] = {
	{"AgroMode", agro_mode},
	{"AgroDemo", agro_demo},
	{"BunkersCount", bunkers_count},
	{"SectionsCount", sections_count},
	{"TowersCount", towers_count},
	{"SemensCount", semens_count},
	{"CropsType", crops_type},
	{"FansCount", fans_count},
	{"FanImpulsCount", fan_impuls_count},
	{"ValsCount", vals_count},
	{"ModCropsCount", mod_crops_count},
	{"ModManureCount", mod_manure_count},
	{"SensorCropsCount", sensor_crops_count},
	{"SensorManureCount", sensor_manure_count},
	{"ControlLevelCrops", control_level_crops},
	{"ControlLevelManure", control_level_manure},
	{"SensorCountTv2", sensor_count_tv2},
	{"isEncoderTv2", is_encoder_tv2},
	{"isFanTv2", is_fan1_tv2},
	{"isFan2Tv2", is_fan2_tv2},
	{"imp2cycleFanTv2", imp2cycle_fan_tv2},
	{"imp2cycleFan2Tv2", imp2cycle_fan2_tv2},
	{"isPressTv2", is_press_tv2},
	{"isPress2Tv2", is_press2_tv2},
	{"isActuatorTv2", is_actuators_tv2},
	{"calcDoubleTv2", calc_double_tv2},
	{"StreemToMsTv2", streem_toms_tv2},
	{"isSowWidth", is_sow_width},
	{"SowWidth", sow_width},
	{"LeftWidth", left_width},
	{"RightWidth", right_width},
	{"SowOver", sow_over},
	{"Gisto_Show", gisto_show},
	{"GistoYellowExist", gisto_yellow_exist},
	{"GistoYellowProcent", gisto_yellow_procent},
	{"GistoRedExist", gisto_red_exist},
	{"GistoRedProcent", gisto_red_procent},
	{"PlugDownInversion", plug_down_inversion},
	{"PlugDownForce", plug_down_force},
	{"DepthSensorsExists", depth_sensors_exists},
	{"DepthSensorsMaskaOn", depth_sensors_maskaon},
	{"FixChangeStream", fix_change_stream},
	{"SensorsCombineCount", sensors_combine_count},
	{"ScrapersCount", scrapers_count},
	{"SensorCombineAdr", sensor_combine_adr},
	{"ScraperWidth", scraper_width},
	{"ScraperDepth", scraper_depth},
	{"ScraperDist", scraper_dist},
	{"CombineSumVol", combine_sum_vol},
	{"SectionExists", section_exists},
	{"SpeedMethod", speed_method},
	{"EncoderConstant", speed_encoder_constant},
	{"EncoderImp2metr", speed_encoder_imp2metr},
	{"EncoderCalibrateControl", encoder_calibrate_control},
	{"ProcentAlarmCalibrateEncoder", procent_alarm_calibrate_encoder},
	{"Fan_Gidro", fan_gidro},
	{"Fan_Dvigatel", fan_dvigatel},
	{"TimeSync", time_sync},
	{"TimeRegion", time_region},
	{"PortContr", port_contr},
	{"PortControl", port_control},
	{"PortSpeed", port_speed},
	{"PaswCommon", pasw_common},
	{NULL, nop_func}
};

static gboolean en = FALSE;
// Parser functions ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static void agro_mode(const struct ParsePack_s *vol) {
	appSettings.AgroMode = (uint8_t)atoi(get_vol(vol, "Value"));
}
static void agro_demo(const struct ParsePack_s *vol) {
	appSettings.AgroDemo = (uint8_t)atoi(get_vol(vol, "Value"));
}
static void bunkers_count(const struct ParsePack_s *vol) {
	appSettings.BunkersCount = atoi(get_vol(vol, "Value"));
}
static void sections_count(const struct ParsePack_s *vol) {
	appSettings.SectionsCount = atoi(get_vol(vol, "Value"));
}
static void towers_count(const struct ParsePack_s *vol) {
	appSettings.TowersCount = atoi(get_vol(vol, "Value"));
}
static void semens_count(const struct ParsePack_s *vol) {
	array_set_int(appSettings.SemensCount, cTowersMaxCount, get_vol(vol, "Value"));
}
static void crops_type(const struct ParsePack_s *vol) {
	array_set_int(appSettings.CropsType, cTowersMaxCount, get_vol(vol, "Value"));
}
static void fans_count(const struct ParsePack_s *vol) {
	appSettings.FansCount = atoi(get_vol(vol, "Value"));
}
static void fan_impuls_count(const struct ParsePack_s *vol) {
	array_set_int(appSettings.FanImpulsCount, cFansMaxCount, get_vol(vol, "Value"));
}
static void vals_count(const struct ParsePack_s *vol) {
	appSettings.ValsCount = atoi(get_vol(vol, "Value"));
}
static void mod_crops_count(const struct ParsePack_s *vol) {
	appSettings.ModCropsCount = atoi(get_vol(vol, "Value"));
}
static void mod_manure_count(const struct ParsePack_s *vol) {
	appSettings.ModManureCount = atoi(get_vol(vol, "Value"));
}
static void sensor_crops_count(const struct ParsePack_s *vol) {
	array_set_int(appSettings.SensorCropsCount, cMaxModules, get_vol(vol, "Value"));
}
static void sensor_manure_count(const struct ParsePack_s *vol) {
	array_set_int(appSettings.SensorManureCount, cMaxModules, get_vol(vol, "Value"));
}
static void control_level_crops(const struct ParsePack_s *vol) {
	appSettings.ControlLevelCrops = (bool)atoi(get_vol(vol, "Value"));
}
static void control_level_manure(const struct ParsePack_s *vol) {
	appSettings.ControlLevelManure = (bool)atoi(get_vol(vol, "Value"));
}
static void sensor_count_tv2(const struct ParsePack_s *vol) {
	appSettings.SensorCountTv2 = atoi(get_vol(vol, "Value"));
}
static void is_encoder_tv2(const struct ParsePack_s *vol) {
	appSettings.isEncoderTv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void is_fan1_tv2(const struct ParsePack_s *vol) {
	appSettings.isFan1Tv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void is_fan2_tv2(const struct ParsePack_s *vol) {
	appSettings.isFan2Tv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void imp2cycle_fan_tv2(const struct ParsePack_s *vol) {
	appSettings.imp2cycleFanTv2 = atoi(get_vol(vol, "Value"));
}
static void imp2cycle_fan2_tv2(const struct ParsePack_s *vol) {
	appSettings.imp2cycleFan2Tv2 = atoi(get_vol(vol, "Value"));
}
static void is_press_tv2(const struct ParsePack_s *vol) {
	appSettings.isPressTv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void is_press2_tv2(const struct ParsePack_s *vol) {
	appSettings.isPress2Tv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void is_actuators_tv2(const struct ParsePack_s *vol) {
	appSettings.isActuatorTv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void calc_double_tv2(const struct ParsePack_s *vol) {
	appSettings.calcDoubleTv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void streem_toms_tv2(const struct ParsePack_s *vol) {
	appSettings.StreemToMsTv2 = (bool)atoi(get_vol(vol, "Value"));
}
static void is_sow_width(const struct ParsePack_s *vol) {
	appSettings.isSowWidth = (bool)atoi(get_vol(vol, "Value"));
}
static void sow_width(const struct ParsePack_s *vol) {
	appSettings.SowWidth = atof(get_vol(vol, "Value"));
}
static void left_width(const struct ParsePack_s *vol) {
	appSettings.LeftWidth = atof(get_vol(vol, "Value"));
}
static void right_width(const struct ParsePack_s *vol) {
	appSettings.RightWidth = atof(get_vol(vol, "Value"));
}
static void sow_over(const struct ParsePack_s *vol) {
	appSettings.SowOver = atof(get_vol(vol, "Value"));
}
static void gisto_show(const struct ParsePack_s *vol) {
	appSettings.Gisto_Show = (bool)atoi(get_vol(vol, "Value"));
}
static void gisto_yellow_exist(const struct ParsePack_s *vol) {
	appSettings.GistoYellowExist = (bool)atoi(get_vol(vol, "Value"));
}
static void gisto_yellow_procent(const struct ParsePack_s *vol) {
	appSettings.GistoYellowProcent = atoi(get_vol(vol, "Value"));
}
static void gisto_red_exist(const struct ParsePack_s *vol) {
	appSettings.GistoRedExist = (bool)atoi(get_vol(vol, "Value"));
}
static void gisto_red_procent(const struct ParsePack_s *vol) {
	appSettings.GistoRedProcent = atoi(get_vol(vol, "Value"));
}
static void plug_down_inversion(const struct ParsePack_s *vol) {
	appSettings.PlugDownInversion = (bool)atoi(get_vol(vol, "Value"));
}
static void plug_down_force(const struct ParsePack_s *vol) {
	appSettings.PlugDownForce = (bool)atoi(get_vol(vol, "Value"));
}
static void depth_sensors_exists(const struct ParsePack_s *vol) {
	appSettings.DepthSensorsExists = (bool)atoi(get_vol(vol, "Value"));
}
static void depth_sensors_maskaon(const struct ParsePack_s *vol) {
	appSettings.DepthSensorsMaskaOn = (uint32_t)atoi(get_vol(vol, "Value"));
}
static void fix_change_stream(const struct ParsePack_s *vol) {
	appSettings.FixChangeStream = (bool)atoi(get_vol(vol, "Value"));
}
static void sensors_combine_count(const struct ParsePack_s *vol) {
	appSettings.SensorsCombineCount = atoi(get_vol(vol, "Value"));
}
static void scrapers_count(const struct ParsePack_s *vol) {
	array_set_int(appSettings.ScrapersCount, cSensorsCombineMaxCount, get_vol(vol, "Value"));
}
static void sensor_combine_adr(const struct ParsePack_s *vol) {
	array_set_int(appSettings.SensorCombineAdr, cSensorsCombineMaxCount, get_vol(vol, "Value"));
}
static void scraper_width(const struct ParsePack_s *vol) {
	appSettings.ScraperWidth = atoi(get_vol(vol, "Value"));
}
static void scraper_depth(const struct ParsePack_s *vol) {
	appSettings.ScraperDepth = atoi(get_vol(vol, "Value"));
}
static void scraper_dist(const struct ParsePack_s *vol) {
	appSettings.ScraperDist = atoi(get_vol(vol, "Value"));
}
static void combine_sum_vol(const struct ParsePack_s *vol) {
	appSettings.CombineSumVol = atoi(get_vol(vol, "Value"));
}
static void section_exists(const struct ParsePack_s *vol) {
	array_set_int(&appSettings.SectionExists[0][0], (cSectionsMaxCount * cSectionItemCount), get_vol(vol, "Value"));
/*
	for(int i0 = 0; i0 < cSectionsMaxCount; i0++)
		for(int i1 = 0; i1 < cSectionItemCount; i1++) {
			if (i1 == 0) puts("");
			printf("%d ", appSettings.SectionExists[i0][i1]);
		}
*/
}
static void speed_method(const struct ParsePack_s *vol) {
	appSettings.SpeedMethod = atoi(get_vol(vol, "Value"));
}
static void speed_encoder_constant(const struct ParsePack_s *vol) {
	appSettings.SpeedEncoderConstant = atof(get_vol(vol, "Value"));
}
static void speed_encoder_imp2metr(const struct ParsePack_s *vol) {
	appSettings.SpeedEncoderImp2metr = atof(get_vol(vol, "Value"));
}
static void encoder_calibrate_control(const struct ParsePack_s *vol) {
	appSettings.EncoderCalibrateControl = (bool)atoi(get_vol(vol, "Value"));
}
static void procent_alarm_calibrate_encoder(const struct ParsePack_s *vol) {
	appSettings.ProcentAlarmCalibrateEncoder = atoi(get_vol(vol, "Value"));
}
static void fan_gidro(const struct ParsePack_s *vol) {
	appSettings.Fan_Gidro = (bool)atoi(get_vol(vol, "Value"));
}
static void fan_dvigatel(const struct ParsePack_s *vol) {
	appSettings.Fan_Dvigatel = (bool)atoi(get_vol(vol, "Value"));
}
static void time_sync(const struct ParsePack_s *vol) {
	appSettings.TimeSync = (bool)atoi(get_vol(vol, "Value"));
}
static void time_region(const struct ParsePack_s *vol) {
	appSettings.TimeRegion = atoi(get_vol(vol, "Value"));
}
static void port_contr(const struct ParsePack_s *vol) {
	if (appSettings.PortContr)
		free(appSettings.PortContr);

	appSettings.PortContr = strdup(get_vol(vol, "Value"));
}
static void port_control(const struct ParsePack_s *vol) {
	if (appSettings.PortControl.dev)
		free(appSettings.PortControl.dev);

	appSettings.PortControl.dev = strdup(get_vol(vol, "dev"));
	appSettings.PortControl.speed = atoi(get_vol(vol, "speed"));
	appSettings.PortControl.pass = (unsigned)atoi(get_vol(vol, "pass"));
	debugf("dev:%s speed:%d pass:%u\n", appSettings.PortControl.dev, appSettings.PortControl.speed, appSettings.PortControl.pass);
}
static void port_speed(const struct ParsePack_s *vol) {
	appSettings.PortSpeed = atoi(get_vol(vol, "Value"));
}
static void pasw_common(const struct ParsePack_s *vol) {
	strncpy(appSettings.PaswCommon, get_vol(vol, "Value"), 64);
}
static void nop_func(const struct ParsePack_s *vol) {
//	printf("Element no function: %s\n", vol->name);
}

// Parser functions --------------------------------------------------------------------------------
static ParsFn select_fn(const char *eName) {
	int inx = 0;
	while(lv[inx].name) {
		if (!strcmp(lv[inx].name, eName))
			break;
		inx++;
	}

	return lv[inx].pars;
}
static void start_element(GMarkupParseContext *ctx, const char *eName, const char **attrName, 
		const char **attrValue, gpointer pData, GError **err) {


	if (!strcmp(eName, "Options")) {
		en = TRUE;
	} else {
		
		struct ParsePack_s pp;

		pp.name = eName;
		pp.aName = attrName;
		pp.aVol = attrValue;

		select_fn(eName)(&pp);
/*

		const char **aName = attrName;
		printf("Start element: %s\n", eName);
		for(int i0 = 0; aName[i0]; i0++) {
			printf("Attr: %s; Vol:%s\n", attrName[i0], attrValue[i0]);
		}
		puts(" ");
*/
	}
}

static void end_element(GMarkupParseContext *ctx, const char *eName, gpointer pData, GError **err) {
	if (!strcmp(eName, "Options")) {
		en = FALSE;
	} else {
//		printf("End element: %s\n", eName);
	}
}

static void text_element(GMarkupParseContext *ctx, const char *txt, gsize len, gpointer pData, GError **err) {
}

static void pass_element(GMarkupParseContext *ctx, const char *txt, gsize len, gpointer pData, GError **err) {
}

static void err_element(GMarkupParseContext *ctx, GError *err, gpointer pData) {
	debug("Error element\n");
}

static void array_set_double(double *arr, uint16_t len, const char *data) {
	char *s1 = strdup(data);
	char *s2;
	int inx = 0;
	s2 = strtok(s1, " ");
	while(s2) {
		if (inx < len) {
			arr[inx++] = atof(s2);
		} else 
			break;

		s2 = strtok(NULL, " ");
	}
	free(s1);
}

static void array_set_int(int *arr, uint16_t len, const char *data) {
	char *s1 = strdup(data);
	char *s2;
	int inx = 0;
	s2 = strtok(s1, " ");
	while(s2) {
		if (inx < len) {
			arr[inx++] = atoi(s2);
		} else 
			break;

		s2 = strtok(NULL, " ");
	}
	free(s1);
}

static const char *get_vol(const struct ParsePack_s *pp, const char *aName) {
	const char *ret = NULL;
	int inx = 0;
	while(pp->aName[inx]) {
		if (!strcmp(pp->aName[inx], aName)) {
			ret = pp->aVol[inx];
			break;
		}
		inx++;
	}
	return ret;
}

const char* speed_Method_ToString(int speedMethod) {
	return(_speedToString[(speedMethod & 3)]);
}

double settings_get_speed() {
	double ret = -1;
	switch(appSettings.SpeedMethod & 3) {
		case 1:  // Get CLONAS method speed
			break;
		case 2: // Get Encoder method speed

			break;
		case 3:
			ret = 7.2;
			break;
	}
	return ret;
}

void read_options(const char *file) {
	gchar *optData;
	gsize len;
	char *oldLoc;

	if (g_file_get_contents(file, &optData, &len, NULL)) {
		oldLoc = strdup(setlocale(LC_NUMERIC, NULL));
		setlocale(LC_NUMERIC, "C");
//		GMarkupParseContext *ctx = g_markup_parse_context_new(&parserOptions, G_MARKUP_DEFAULT_FLAGS, NULL, NULL);
		GMarkupParseContext *ctx = g_markup_parse_context_new(&parserOptions, G_MARKUP_DO_NOT_USE_THIS_UNSUPPORTED_FLAG, NULL, NULL);
		en = TRUE;

		while(g_markup_parse_context_parse(ctx, optData, len, NULL))
			if (g_markup_parse_context_end_parse(ctx, NULL)) break;

		g_markup_parse_context_free(ctx);
		setlocale(LC_NUMERIC, oldLoc);
		free(oldLoc);
		g_free(optData);
	} else {
		debugf("Ошибка чтения файла настроек: %s\n", file);
	}
}

