/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include "bs_types.h"
#include "bs_tracing.h"
#include "bs_utils.h"
#include "bstests.h"

#define WAIT_TIME 10 /* Seconds */

extern enum bst_result_t bst_result;

#define FAIL(...)                                                                                  \
	do {                                                                                       \
		bst_result = Failed;                                                               \
		bs_trace_error_time_line(__VA_ARGS__);                                             \
	} while (0)

#define PASS(...)                                                                                  \
	do {                                                                                       \
		bst_result = Passed;                                                               \
		bs_trace_info_time(1, __VA_ARGS__);                                                \
	} while (0)

static void test_ccp_call_control_client_sample_init(void)
{
	bst_ticker_set_next_tick_absolute(WAIT_TIME * 1e6);
	bst_result = In_progress;
}

static void test_ccp_call_control_client_sample_tick(bs_time_t HW_device_time)
{
	extern struct bt_ccp_call_control_client *client;

	/* If discovery was a success then client is non-NULL - Use as pass criteria */
	if (client == NULL) {
		FAIL("CCP Call Control Client sample FAILED (Did not pass after %i seconds)\n",
		     WAIT_TIME);
	} else {
		PASS("CCP Call Control Client sample PASSED\n");
	}
}

static const struct bst_test_instance test_sample[] = {
	{
		.test_id = "ccp_call_control_client",
		.test_descr = "Test based on the CCP Call Control Client sample. "
			      "It expects to be connected to a compatible CCP server, "
			      "waits for " STR(WAIT_TIME) " seconds, and checks how "
			      "many audio packets have been received correctly",
		.test_post_init_f = test_ccp_call_control_client_sample_init,
		.test_tick_f = test_ccp_call_control_client_sample_tick,
	},
	BSTEST_END_MARKER,
};

static struct bst_test_list *
test_ccp_call_control_client_sample_install(struct bst_test_list *tests)
{
	tests = bst_add_tests(tests, test_sample);
	return tests;
}

bst_test_install_t test_installers[] = {test_ccp_call_control_client_sample_install, NULL};
