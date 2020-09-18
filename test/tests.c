/*
 * tests.c
 *
 *  Created on: Sep 17, 2020
 *      Author: Burak
 */

#include "../unity/unity_fixture.h"
#include "main.h"
#include "modbus_rtu.c"

//ne test edilecekse onun icin bir grup olusturulur
TEST_GROUP(modbus);

//her testten once calistirilacak kod buraya yazilir
TEST_SETUP(modbus){

}

//her testten sonra calistirilacak kod buraya yazilir
TEST_TEAR_DOWN(modbus){

}

TEST(modbus, crc16_return_expected_test){

	uint8_t testMsg[] = {0x01,0x04,0x02,0x01,0x31};
	TEST_ASSERT_EQUAL(29817, calc_modbus_crc16(testMsg,5));
	uint8_t testMsg1[] = {0x00,0x00,0x00,0x00,0x00};
	TEST_ASSERT_EQUAL(36, calc_modbus_crc16(testMsg1,5));
	uint8_t testMsg2[256] = {0};
	for(int i = 0; i< 256; i++)
	{
		testMsg2[i] = i;
	}
	TEST_ASSERT_EQUAL(56940, calc_modbus_crc16(testMsg2,256));

}

TEST(modbus, modbus_is_package_started_test){

	HAL_Delay(4);

	TEST_ASSERT_EQUAL(1, modbus_is_package_started(3));

	TEST_ASSERT_EQUAL(0, modbus_is_package_started(3));

	HAL_Delay(4);

	TEST_ASSERT_EQUAL(1, modbus_is_package_started(3));
}

TEST(modbus, detect_whole_package_test){
	HAL_Delay(4);
	uint8_t testMsg[] = {0x01,0x04,0x02,0x01,0x31,0x79,0x74};

	for(int i = 0; i < sizeof(testMsg) - 1; i++)
	{
		TEST_ASSERT_EQUAL_PTR(NULL,detect_whole_package(testMsg[i]));
	}
	TEST_ASSERT_EQUAL_MEMORY(testMsg, detect_whole_package(testMsg[sizeof(testMsg) - 1]) , sizeof(testMsg));

}

TEST(modbus, detect_whole_package_test_10byte){
	HAL_Delay(4);
	uint8_t testMsg[] = {0x01, 0x05, 0x06, 0x65, 0x32, 0x14, 0x78, 0x96, 0x74, 0x7b};

	for(int i = 0; i < sizeof(testMsg) - 1; i++)
	{
		TEST_ASSERT_EQUAL_PTR(NULL,detect_whole_package(testMsg[i]));
	}
	TEST_ASSERT_EQUAL_MEMORY(testMsg, detect_whole_package(testMsg[sizeof(testMsg) - 1]) , sizeof(testMsg));

	uint8_t testMsg1[] = {0x01, 0x05, 0x06, 0x65, 0x32, 0x14, 0x78, 0x96, 0x74, 0x7b};

	for(int i = 0; i < sizeof(testMsg1) - 1; i++)
	{
		TEST_ASSERT_EQUAL_PTR(NULL,detect_whole_package(testMsg1[i]));
	}
	TEST_ASSERT_EQUAL_PTR(NULL,detect_whole_package(testMsg1[sizeof(testMsg) - 1]));
}

TEST(modbus, get_slave_id_from_pck_test){

	uint8_t testMsg[] = {0x01,0x04,0x02,0x01,0x31,0x79,0x74};
	TEST_ASSERT_EQUAL_PTR(0x01,get_slave_id_from_pck(testMsg));
	uint8_t testMsg1[] = {0x05,0x04,0x02,0x01,0x31,0x79,0x74};
	TEST_ASSERT_EQUAL_PTR(0x05,get_slave_id_from_pck(testMsg1));
	uint8_t testMsg2[] = {0xFD,0x04,0x02,0x01,0x31,0x79,0x74};
	TEST_ASSERT_EQUAL_PTR(0xFF,get_slave_id_from_pck(testMsg2));

}

TEST(modbus, get_function_id_from_pck){

	uint8_t testMsg[] = {0x01,0x04,0x02,0x01,0x31,0x79,0x74};
	TEST_ASSERT_EQUAL_PTR(0x04,get_function_id_from_pck(testMsg));
	uint8_t testMsg1[] = {0x05,0x10,0x02,0x01,0x31,0x79,0x74};
	TEST_ASSERT_EQUAL_PTR(0x10,get_function_id_from_pck(testMsg1));
	uint8_t testMsg2[] = {0xFD,0xAA,0x02,0x01,0x31,0x79,0x74};
	TEST_ASSERT_EQUAL_PTR(0xFF,get_function_id_from_pck(testMsg2));

}



TEST_GROUP_RUNNER(modbus){
	RUN_TEST_CASE(modbus, crc16_return_expected_test);
	RUN_TEST_CASE(modbus, modbus_is_package_started_test);
	RUN_TEST_CASE(modbus, detect_whole_package_test);
	RUN_TEST_CASE(modbus, detect_whole_package_test_10byte);
	RUN_TEST_CASE(modbus, get_slave_id_from_pck_test);
	RUN_TEST_CASE(modbus, get_function_id_from_pck)
	//diger testler sirasiyla buraya yazilir.
}

void run_all_tests(){
	//hangi gruptaki testleri calistiracaksak buradan belirliyoruz.
	//asagidaki kodu yorumla devre disi birakirsak bu grupta yazilan kodlar calisir.
	RUN_TEST_GROUP(modbus);
}

void start_test(void){

	//
	UnityMain(1, 0, run_all_tests);

	for(;;){
		//sistemi durdur...
	}
}
