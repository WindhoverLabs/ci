#ifndef SRC_CI_SERIALIZATION_H_
#define SRC_CI_SERIALIZATION_H_
#endif

#include "cfe.h"
#include "ci_private_types.h"
#include "ci_msg.h"

#include "../../../../tools/nanopb/pb_encode.h"
#include "../../../../tools/nanopb/pb_decode.h"
#include "ci_reg.pb.h"

/* Serialization Functions */
uint32 CI_CmdRegData_Enc(const CI_CmdRegData_t *inObject, char *inOutBuffer, uint32 inSize);
uint32 CI_CmdRegData_Dec(const char *inBuffer, uint32 inSize, CI_CmdRegData_t *inOutObject);
