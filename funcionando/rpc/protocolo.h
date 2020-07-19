/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _PROTOCOLO_H_RPCGEN
#define _PROTOCOLO_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	u_int Mensaje_len;
	char *Mensaje_val;
} Mensaje;

#define PROY2DFS 0x20001003
#define PROY2DFSVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define LS 1
extern  Mensaje * ls_1(Mensaje *, CLIENT *);
extern  Mensaje * ls_1_svc(Mensaje *, struct svc_req *);
#define GETADDRESS 2
extern  Mensaje * getaddress_1(Mensaje *, CLIENT *);
extern  Mensaje * getaddress_1_svc(Mensaje *, struct svc_req *);
#define EXISTS 3
extern  int * exists_1(Mensaje *, CLIENT *);
extern  int * exists_1_svc(Mensaje *, struct svc_req *);
#define UPDATE_ADDRESS 4
extern  Mensaje * update_address_1(Mensaje *, CLIENT *);
extern  Mensaje * update_address_1_svc(Mensaje *, struct svc_req *);
#define REPORT_CREATE 5
extern  int * report_create_1(Mensaje *, CLIENT *);
extern  int * report_create_1_svc(Mensaje *, struct svc_req *);
#define REPORT_DELETE 6
extern  int * report_delete_1(Mensaje *, CLIENT *);
extern  int * report_delete_1_svc(Mensaje *, struct svc_req *);
#define IS_EMPTY 7
extern  int * is_empty_1(Mensaje *, CLIENT *);
extern  int * is_empty_1_svc(Mensaje *, struct svc_req *);
extern int proy2dfs_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define LS 1
extern  Mensaje * ls_1();
extern  Mensaje * ls_1_svc();
#define GETADDRESS 2
extern  Mensaje * getaddress_1();
extern  Mensaje * getaddress_1_svc();
#define EXISTS 3
extern  int * exists_1();
extern  int * exists_1_svc();
#define UPDATE_ADDRESS 4
extern  Mensaje * update_address_1();
extern  Mensaje * update_address_1_svc();
#define REPORT_CREATE 5
extern  int * report_create_1();
extern  int * report_create_1_svc();
#define REPORT_DELETE 6
extern  int * report_delete_1();
extern  int * report_delete_1_svc();
#define IS_EMPTY 7
extern  int * is_empty_1();
extern  int * is_empty_1_svc();
extern int proy2dfs_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Mensaje (XDR *, Mensaje*);

#else /* K&R C */
extern bool_t xdr_Mensaje ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_PROTOCOLO_H_RPCGEN */
