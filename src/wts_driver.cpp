/**
 * @file   wts_driver.cpp
 * @author Chittaranjan S Srinivas
 *
 * @brief  This file defines the WTSDriver class.
 *
 * Copyright (C) 2016  Chittaranjan Srinivas Swaminathan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

#include <wts_driver/wts_driver.hpp>

namespace wts_driver {

const uint16_t WTSDriver::crc_table[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
  0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
  0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
  0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
  0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
  0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
  0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
  0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
  0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
  0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
  0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
  0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
  0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
  0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
  0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
  0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
  0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
  0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
  0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
  0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
  0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
  0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
  0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

WTSDriver::WTSDriver(SerialComm& serial_comm) :
  serial_comm_(serial_comm) {


}

WTSDriver::~WTSDriver() {
}

uint16_t WTSDriver::calculateCRC(const std::vector<uint8_t>& data, uint16_t crc_prev) {

  uint16_t crc = crc_prev;

  for(std::vector<uint8_t>::const_iterator it = data.begin(); it != data.end(); it++) {
    crc = crc_table[(crc ^ (*it)) & 0xFF] ^ (crc >> 8);
  }

  return crc;
}

void WTSDriver::appendPreambleCommandSize(const wts_command::command_type cmd_type, const uint16_t size, std::vector <uint8_t>& command_message) {

  /* Preamble */
  command_message.push_back(0xaa);command_message.push_back(0xaa);command_message.push_back(0xaa);

  /* ID */
  command_message.push_back(static_cast<uint8_t>(cmd_type));

  uint8_t size2 = size & 0xFF;
  uint8_t size1 = size >> 8;

  /* Size */
  command_message.push_back(size2);command_message.push_back(size1);
}

wts_error::error_type WTSDriver::getSensorType(std::string& sensor_type) {

  // First assemble the request message.
  std::vector <uint8_t> command_message;

  appendPreambleCommandSize(wts_command::GET_SENSOR_TYPE, 0x0000, command_message);

  uint16_t checksum = calculateCRC(command_message);

  std::vector <boost::asio::const_buffer> buffersToWrite;
  buffersToWrite.push_back(boost::asio::buffer(command_message));
  buffersToWrite.push_back(boost::asio::buffer(&checksum, sizeof(checksum)));

  serial_comm_.writeConstBufferSequence(buffersToWrite);

  // Now read acknowledgement synchronously. Note: This isn't thread safe.
  std::vector <uint8_t> returned_parameters;
  wts_error::error_type error = readAcknowledgement(wts_command::GET_SENSOR_TYPE, returned_parameters);
  sensor_type = std::string(returned_parameters.begin(), returned_parameters.end());
  return error;
}

wts_error::error_type WTSDriver::getDeviceTag(std::string& device_tag) {

  // First assemble the request message.
  std::vector <uint8_t> command_message;

  appendPreambleCommandSize(wts_command::GET_DEVICE_TAG, 0x0000, command_message);

  uint16_t checksum = calculateCRC(command_message);

  std::vector <boost::asio::const_buffer> buffersToWrite;
  buffersToWrite.push_back(boost::asio::buffer(command_message));
  buffersToWrite.push_back(boost::asio::buffer(&checksum, sizeof(checksum)));

  serial_comm_.writeConstBufferSequence(buffersToWrite);

  // Now read acknowledgement synchronously. Note: This isn't thread safe.
  std::vector <uint8_t> returned_parameters;
  wts_error::error_type error = readAcknowledgement(wts_command::GET_DEVICE_TAG, returned_parameters);
  device_tag = std::string(returned_parameters.begin(), returned_parameters.end());
  return error;

}

wts_error::error_type WTSDriver::readDeviceTemperature(int& temperature) {

  // First assemble the request message.
  std::vector <uint8_t> command_message;

  appendPreambleCommandSize(wts_command::READ_DEVICE_TEMPERATURE, 0x0000, command_message);

  uint16_t checksum = calculateCRC(command_message);

  std::vector <boost::asio::const_buffer> buffersToWrite;
  buffersToWrite.push_back(boost::asio::buffer(command_message));
  buffersToWrite.push_back(boost::asio::buffer(&checksum, sizeof(checksum)));

  serial_comm_.writeConstBufferSequence(buffersToWrite);

  // Now read acknowledgement synchronously. Note: This isn't thread safe.
  std::vector <uint8_t> returned_parameters;
  wts_error::error_type error = readAcknowledgement(wts_command::READ_DEVICE_TEMPERATURE, returned_parameters);
  temperature = returned_parameters[0] | (returned_parameters[1] << 8);
  temperature /= 10;
  return error;

}

wts_error::error_type WTSDriver::getSystemInformation() {

  // First assemble the request message.
  std::vector <uint8_t> command_message;

  appendPreambleCommandSize(wts_command::GET_SYSTEM_INFO, 0x0000, command_message);

  uint16_t checksum = calculateCRC(command_message);

  std::vector <boost::asio::const_buffer> buffersToWrite;
  buffersToWrite.push_back(boost::asio::buffer(command_message));
  buffersToWrite.push_back(boost::asio::buffer(&checksum, sizeof(checksum)));

  serial_comm_.writeConstBufferSequence(buffersToWrite);

  // Now read acknowledgement synchronously. Note: This isn't thread safe.
  std::vector <uint8_t> returned_parameters;
  wts_error::error_type error = readAcknowledgement(wts_command::GET_SYSTEM_INFO, returned_parameters);

  system_info = SystemInfo(returned_parameters);
  return error;

}

wts_error::error_type WTSDriver::readSingleFrame(Frame& frame, bool compression) {

  // First assemble the request message.
  std::vector <uint8_t> command_message;

  appendPreambleCommandSize(wts_command::READ_SINGLE_FRAME, 0x0000, command_message);

  uint16_t checksum = calculateCRC(command_message);

  std::vector <boost::asio::const_buffer> buffersToWrite;
  buffersToWrite.push_back(boost::asio::buffer(command_message));
  buffersToWrite.push_back(boost::asio::buffer(&checksum, sizeof(checksum)));

  serial_comm_.writeConstBufferSequence(buffersToWrite);

  // Now read acknowledgement synchronously. Note: This isn't thread safe.
  std::vector <uint8_t> returned_parameters;
  wts_error::error_type error = readAcknowledgement(wts_command::READ_SINGLE_FRAME, returned_parameters);

  // TODO: Incomplete

  return error;

}

wts_error::error_type WTSDriver::getMatrixInformation() {

  // First assemble the request message.
  std::vector <uint8_t> command_message;

  appendPreambleCommandSize(wts_command::GET_MATRIX_INFO, 0x0000, command_message);

  uint16_t checksum = calculateCRC(command_message);

  std::vector <boost::asio::const_buffer> buffersToWrite;
  buffersToWrite.push_back(boost::asio::buffer(command_message));
  buffersToWrite.push_back(boost::asio::buffer(&checksum, sizeof(checksum)));

  serial_comm_.writeConstBufferSequence(buffersToWrite);

  // Now read acknowledgement synchronously. Note: This isn't thread safe.
  std::vector <uint8_t> returned_parameters;
  wts_error::error_type error = readAcknowledgement(wts_command::GET_MATRIX_INFO, returned_parameters);

  resolution_x = (returned_parameters[0]) | (returned_parameters[1] << 8);
  resolution_y = (returned_parameters[2]) | (returned_parameters[3] << 8);
  cell_width = ( (returned_parameters[4]) | (returned_parameters[5] << 8) ) / 100000.00;
  cell_height = ( (returned_parameters[6]) | (returned_parameters[7] << 8) ) / 100000.00;
  full_scale_output = (returned_parameters[8]) | (returned_parameters[9] << 8);

  return error;
}

void WTSDriver::displayMatrixInformation() {
  printf("\nResolution X: %d\nResolution Y: %d\nWidth: %f m\nHeight: %f m\nFull Scale Output: %d",
      resolution_x,
      resolution_y,
      cell_width,
      cell_height,
      full_scale_output);
}

void WTSDriver::displaySystemInformation() {
  system_info.display();
}


wts_error::error_type WTSDriver::readAcknowledgement(const wts_command::command_type cmd_type, std::vector <uint8_t>& returned_parameters) {

  /* First read preamble, command id and the size */
    std::vector <uint8_t> readBytes1;
    readBytes1.resize(6);

    serial_comm_.readBytes(readBytes1);
    wts_command::command_type cmd_id_returned = static_cast<wts_command::command_type> (readBytes1[3]);
    uint16_t receivedSize = readBytes1[4] | (readBytes1[5] << 8);

    if(cmd_id_returned != cmd_type) {
      throw ReceivedUnexpectedCommandIDException();
      return wts_error::E_OTHER;
    }

    // We have received the right response. Lets make sure the command succeeded.
    uint16_t status_code;
    serial_comm_.readFromSerialPort(status_code);

    if(status_code != wts_error::E_SUCCESS) {
      return static_cast<wts_error::error_type> (status_code);
    }

    // If we get here, we can safely read the data.
    // We read the response alone now.
    returned_parameters.resize(receivedSize - 2);
    serial_comm_.readBytes(returned_parameters);

    // We will read the checksum and ignore it.
    uint16_t checksumReceived;
    serial_comm_.readFromSerialPort(checksumReceived);


    // We append the status code to readBytes1 for checksum computation.
    readBytes1.push_back(status_code & 0xFF);
    readBytes1.push_back(status_code >> 8);

    uint16_t checksumComputed = calculateCRC(readBytes1);
    checksumComputed = calculateCRC(returned_parameters, checksumComputed);

    if(checksumComputed != checksumReceived) {
      std::cout << "{WARNING}:[GET_SENSOR_TYPE]: The checksums don't match!";
    }

    return static_cast<wts_error::error_type> (status_code);

}

} /* namespace wts */
