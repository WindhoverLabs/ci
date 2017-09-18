#include "ci_serialization.h"

uint32 CI_CmdRegData_Enc(const CI_CmdRegData_t *inObject, char *inOutBuffer, uint32 inSize)
{
	bool status = false;
	ci_register_pb pbMsg;
	
    pbMsg.msgID = inObject->msgID;
    pbMsg.cmdCode = inObject->cmdCode;
    pbMsg.step = inObject->step;
    pbMsg.log = inObject->log;

	/* Create a stream that will write to our buffer. */
	pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t *)inOutBuffer, inSize);

	/* Now we are ready to encode the message. */
	status = pb_encode(&stream, ci_register_pb_fields, &pbMsg);

	/* Check for errors... */
	if (!status)
	{
		return 0;
	}

	return stream.bytes_written;
}

uint32 CI_CmdRegData_Dec(const char *inBuffer, uint32 inSize, CI_CmdRegData_t *inOutObject)
{
	bool status = false;
	ci_register_pb pbMsg;

	/* Create a stream that reads from the buffer. */
	pb_istream_t stream = pb_istream_from_buffer((const pb_byte_t *)inBuffer, inSize);

	/* Now we are ready to decode the message. */
	status = pb_decode(&stream, ci_register_pb_fields, &pbMsg);

	/* Check for errors... */
	if (!status)
	{
		return 0;
	}

    inOutObject->msgID = pbMsg.msgID;
    inOutObject->cmdCode = pbMsg.cmdCode;
    inOutObject->step = pbMsg.step;
    inOutObject->log = pbMsg.log;

	return sizeof(CI_CmdRegData_t);
}
