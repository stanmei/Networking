/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "get_file.h"

bool_t
xdr_get_file_content_1_argument (XDR *xdrs, get_file_content_1_argument *objp)
{
	 if (!xdr_string (xdrs, &objp->arg1, ~0))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->arg2))
		 return FALSE;
	return TRUE;
}