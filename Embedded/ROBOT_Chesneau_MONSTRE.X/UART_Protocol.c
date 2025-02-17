#include <xc.h>
#include "UART_Protocol.h"
#include "CB_TX1.h"
unsigned char UartCalculateChecksum(int msgFunction,
int msgPayloadLength, unsigned char* msgPayload)
    {
        unsigned char checksum = 0;
        checksum ^= 0xFE;
        checksum ^= (unsigned char)(msgFunction >> 8);
        checksum ^= (unsigned char)(msgFunction >> 0);
        checksum ^= (unsigned char)(msgPayloadLength >> 8);
        checksum ^= (unsigned char)(msgPayloadLength >> 0);
        for (int i=0; i< msgPayloadLength; i++)
            {
                checksum ^= msgPayload[i];
            }
                

        return checksum;
    }
void UartEncodeAndSendMessage(int msgFunction,
int msgPayloadLength, unsigned char* msgPayload)
    {
        unsigned char message[msgPayloadLength+6];
        int pos = 0;
        message[pos++] = 0xFE;
        message[pos++] = (unsigned char)(msgFunction >> 8);
        message[pos++] = (unsigned char)(msgFunction >> 0);
        message[pos++] = (unsigned char)(msgPayloadLength >> 8);
        message[pos++] = (unsigned char)(msgPayloadLength >> 0);
        for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] ^= msgPayload[i];
            }

        unsigned char checksum = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
        message[pos++] = (unsigned char)(checksum);
        SendMessage(message,msgPayloadLength+6);
    }
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0; 
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;
void UartDecodeMessage(unsigned char c)
        {
    switch (rcvState)
{
    case StateReception.Waiting:
        if (c == 0xFE)
        {
            rcvState = StateReception.FunctionMSB;
            i = 0;
        }
        break;
    case StateReception.FunctionMSB:
        msgDecodedFunction = c <<8;
        rcvState = StateReception.FunctionLSB;
        break;
    case StateReception.FunctionLSB:
        msgDecodedFunction |= c;
        rcvState = StateReception.PayloadLengthMSB;
        break;
    case StateReception.PayloadLengthMSB:
        msgDecodedPayloadLength = c<<8;
        rcvState = StateReception.PayloadLengthLSB;
        break;
    case StateReception.PayloadLengthLSB:
        msgDecodedPayloadLength |= c;
        rcvState = StateReception.Payload;
        if (msgDecodedPayloadLength == 0)
        {
            rcvState = StateReception.CheckSum;
        }
        else
        {
            msgDecodedPayload = new byte[msgDecodedPayloadLength];
            msgDecodedPayloadIndex = 0;
            rcvState = StateReception.Payload;
        }

        break;
    case StateReception.Payload:
        msgDecodedPayload[msgDecodedPayloadIndex++] = c;
        if (msgDecodedPayloadIndex >= msgDecodedPayloadLength)
            rcvState = StateReception.CheckSum;
        break;
    case StateReception.CheckSum:
        receivedChecksum = c;
        calculatedChecksum = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
        if (calculatedChecksum == receivedChecksum)
        {
            //Success, on a un message valide
            ProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
        }
        else
        {
            ReceptionTextBox.Text += "Bouououoouh";
        }
        rcvState = StateReception.Waiting;
        break;
    default:
    rcvState = StateReception.Waiting;
    break;
}
        }
void UartProcessDecodedMessage(int function,
int payloadLength, unsigned char* payload)
    {
    //Fonction appelee apres le decodage pour executer l?action
    //correspondant au message recu
    ...
    }
    //*************************************************************************/
    //Fonctions correspondant aux messages
    //*************************************************************************/