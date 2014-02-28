/*
 * Copyright (C) 2014 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include<stdio.h>
#include<ctype.h>

#include<hardware/bluetooth.h>
#include<hardware/bt_hh.h>

#include "if-main.h"
#include "pollhandler.h"
#include "../hal-utils.h"

const btrc_interface_t *if_rc = NULL;

SINTMAP(btrc_play_status_t, -1, "(unknown)")
	DELEMENT(BTRC_PLAYSTATE_STOPPED),
	DELEMENT(BTRC_PLAYSTATE_PLAYING),
	DELEMENT(BTRC_PLAYSTATE_PAUSED),
	DELEMENT(BTRC_PLAYSTATE_FWD_SEEK),
	DELEMENT(BTRC_PLAYSTATE_REV_SEEK),
	DELEMENT(BTRC_PLAYSTATE_ERROR),
ENDMAP

SINTMAP(btrc_media_attr_t, -1, "(unknown)")
	DELEMENT(BTRC_MEDIA_ATTR_TITLE),
	DELEMENT(BTRC_MEDIA_ATTR_ARTIST),
	DELEMENT(BTRC_MEDIA_ATTR_ALBUM),
	DELEMENT(BTRC_MEDIA_ATTR_TRACK_NUM),
	DELEMENT(BTRC_MEDIA_ATTR_NUM_TRACKS),
	DELEMENT(BTRC_MEDIA_ATTR_GENRE),
	DELEMENT(BTRC_MEDIA_ATTR_PLAYING_TIME),
ENDMAP

static btrc_callbacks_t rc_cbacks = {
	.size = sizeof(rc_cbacks),
};

/* init */

static void init_p(int argc, const char **argv)
{
	RETURN_IF_NULL(if_rc);

	EXEC(if_rc->init, &rc_cbacks);
}

/* get_play_status_rsp */

static void get_play_status_rsp_c(int argc, const char **argv,
					enum_func *enum_func, void **user)
{
	if (argc == 3) {
		*user = TYPE_ENUM(btrc_play_status_t);
		*enum_func = enum_defines;
	}
}

static void get_play_status_rsp_p(int argc, const char **argv)
{
	btrc_play_status_t play_status;
	uint32_t song_len, song_pos;

	RETURN_IF_NULL(if_rc);

	if (argc <= 2) {
		haltest_error("No play status specified");
		return;
	}

	if (argc <= 3) {
		haltest_error("No song length specified");
		return;
	}

	if (argc <= 4) {
		haltest_error("No song position specified");
		return;
	}

	play_status = str2btrc_play_status_t(argv[2]);
	song_len = (uint32_t) atoi(argv[3]);
	song_pos = (uint32_t) atoi(argv[4]);

	EXEC(if_rc->get_play_status_rsp, play_status, song_len, song_pos);
}

/* get_element_attr_rsp */

static void get_element_attr_rsp_c(int argc, const char **argv,
					enum_func *enum_func, void **user)
{
	if (argc == 4) {
		*user = TYPE_ENUM(btrc_media_attr_t);
		*enum_func = enum_defines;
	}
}

static void get_element_attr_rsp_p(int argc, const char **argv)
{
	uint8_t num_attr;
	btrc_element_attr_val_t attrs;

	RETURN_IF_NULL(if_rc);

	if (argc <= 2) {
		haltest_error("No number of attributes specified");
		return;
	}

	if (argc <= 4) {
		haltest_error("No attr id and value specified");
		return;
	}

	num_attr = (uint8_t) atoi(argv[2]);
	attrs.attr_id = str2btrc_media_attr_t(argv[3]);
	strcpy((char *)attrs.text, argv[4]);

	EXEC(if_rc->get_element_attr_rsp, num_attr, &attrs);
}

/* cleanup */

static void cleanup_p(int argc, const char **argv)
{
	RETURN_IF_NULL(if_rc);

	EXECV(if_rc->cleanup);
	if_rc = NULL;
}

static struct method methods[] = {
	STD_METHOD(init),
	STD_METHODCH(get_play_status_rsp,
					"<play_status> <song_len> <song_pos>"),
	STD_METHODCH(get_element_attr_rsp, "<num_attr> <attrs_id> <value>"),
	STD_METHOD(cleanup),
	END_METHOD
};

const struct interface rc_if = {
	.name = "rc",
	.methods = methods
};
