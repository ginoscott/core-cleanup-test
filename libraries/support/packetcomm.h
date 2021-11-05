#ifndef PACKETCOMM_H
#define PACKETCOMM_H

#include "support/configCosmos.h"
#include "math/mathlib.h"
#include "support/enumlib.h"
#include "support/transferclass.h"

namespace Cosmos {
    namespace Support {
        class PacketComm
        {
        public:
            struct __attribute__ ((packed)) CCSDS_Header {
                uint8_t tf_version:2;
                uint16_t spacecraft_id:10;
                uint8_t virtual_channel_id:3;
                uint8_t ocf_flag:1;
                uint8_t master_frame_cnt;
                uint8_t virtual_frame_cnt; // Start frame, will increment
                uint16_t tf_data_field_status;
            };

            PacketComm();
            void CalcCRC();
            bool CheckCRC();
            bool Unpack(bool checkcrc=true);
            bool UnpackForward();
            bool RawIn(bool invert=false, bool checkcrc=true);
            bool CCSDSIn();
            bool ASMIn();
            bool SLIPIn();
            bool Pack();
            bool RawOut();
            bool ASMOut();
            bool AX25Out(string dest_call="", string sour_call="", uint8_t dest_stat=0x60, uint8_t sour_stat=0x61, uint8_t cont=0x03, uint8_t prot=0xf0);
            bool SLIPOut();
            int32_t Process();
            int32_t Generate(string args="");

            static constexpr uint16_t INTERNAL_BEACON = 0;
            static constexpr uint16_t INTERNAL_COMMAND = 0;
            static constexpr uint16_t EXTERNAL_COMMAND = 14;

            Enum TypeId;
            typedef int32_t (*Func)(PacketComm *packet, string args);
            Func Funcs[256];

            CCSDS_Header ccsds_header;
            vector<uint8_t> dataout;
            vector<uint8_t> datain;
            uint8_t type;
			/// Data of interest
            vector<uint8_t> data;
            uint16_t crc;
            // Destination for forward type packets
            string fdest;

            struct __attribute__ ((packed)) FileData
            {
                uint16_t size;
                uint32_t txid;
            };

            struct FileMeta
            {

            };

        private:
            vector<uint8_t> atsm = {0x1a, 0xcf, 0xfc, 0x1d};
            vector<uint8_t> atsmr = {0x58, 0xf3, 0x3f, 0xb8};
            CRC16 calc_crc;

            Transfer ttransfer;
            int32_t close_transfer();

            bool PackForward();


            static int32_t ShortReset(PacketComm *packet, string args="");
            static int32_t ShortReboot(PacketComm *packet, string args="");
            static int32_t ShortSendBeacon(PacketComm *packet, string args="");
            static int32_t LongReset(PacketComm *packet, string args="");
            static int32_t LongReboot(PacketComm *packet, string args="");
            static int32_t LongSendBeacon(PacketComm *packet, string args="");
            static int32_t FileMeta(PacketComm *packet, string args="");
            static int32_t FileChunk(PacketComm *packet, string args="");

            static int32_t ShortCPUBeacon(PacketComm *packet, string args="");
            static int32_t ShortTempBeacon(PacketComm *packet, string args="");
            static int32_t LongCPUBeacon(PacketComm *packet, string args="");
            static int32_t LongTempBeacon(PacketComm *packet, string args="");
        };
    }
}

#endif // PACKETCOMM_H
