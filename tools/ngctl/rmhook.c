
/*
 * rmhook.c
 *
 * Copyright (c) 1996-1999 Whistle Communications, Inc.
 * All rights reserved.
 *
 * Subject to the following obligations and disclaimer of warranty, use and
 * redistribution of this software, in source or object code forms, with or
 * without modifications are expressly permitted by Whistle Communications;
 * provided, however, that:
 * 1. Any and all reproductions of the source or object code must include the
 *    copyright notice above and the following disclaimer of warranties; and
 * 2. No rights are granted, in any manner or form, to use Whistle
 *    Communications, Inc. trademarks, including the mark "WHISTLE
 *    COMMUNICATIONS" on advertising, endorsements, or otherwise except as
 *    such appears in the above copyright notice or in the software.
 *
 * THIS SOFTWARE IS BEING PROVIDED BY WHISTLE COMMUNICATIONS "AS IS", AND
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, WHISTLE COMMUNICATIONS MAKES NO
 * REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, REGARDING THIS SOFTWARE,
 * INCLUDING WITHOUT LIMITATION, ANY AND ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
 * WHISTLE COMMUNICATIONS DOES NOT WARRANT, GUARANTEE, OR MAKE ANY
 * REPRESENTATIONS REGARDING THE USE OF, OR THE RESULTS OF THE USE OF THIS
 * SOFTWARE IN TERMS OF ITS CORRECTNESS, ACCURACY, RELIABILITY OR OTHERWISE.
 * IN NO EVENT SHALL WHISTLE COMMUNICATIONS BE LIABLE FOR ANY DAMAGES
 * RESULTING FROM OR ARISING OUT OF ANY USE OF THIS SOFTWARE, INCLUDING
 * WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * PUNITIVE, OR CONSEQUENTIAL DAMAGES, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES, LOSS OF USE, DATA OR PROFITS, HOWEVER CAUSED AND UNDER ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF WHISTLE COMMUNICATIONS IS ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include <err.h>
#include <netgraph.h>
#include <stdio.h>

#include "ngctl.h"

static int RmHookCmd(int ac, char **av);

const struct ngcmd rmhook_cmd = {
	RmHookCmd,
	"rmhook [path] <hook>",
	"Disconnect hook \"hook\" of the node at \"path\"",
	"The rmhook command forces the node at \"path\" to break the link"
	" formed by its hook \"hook\", if connected."
	" If \"path\" is omitted then \".\" is assumed.",
	{ "disconnect" }
};

static int
RmHookCmd(int ac, char **av)
{
	struct ngm_rmhook rmh;
	const char *path = ".";

	/* Get arguments */
	switch (ac) {
	case 3:
		path = av[1];
		ac--;
		av++;
		/* FALLTHROUGH */
	case 2:
		snprintf(rmh.ourhook, sizeof(rmh.ourhook), "%s", av[1]);
		break;
	default:
		return (CMDRTN_USAGE);
	}

	/* Send message */
	if (NgSendMsg(csock, path, NGM_GENERIC_COOKIE,
	    NGM_RMHOOK, &rmh, sizeof(rmh)) < 0) {
		warn("send msg");
		return (CMDRTN_ERROR);
	}
	return (CMDRTN_OK);
}

