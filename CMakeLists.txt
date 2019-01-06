CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

PROJECT(backtrader)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wall -fPIC")

IF (WIN32)
	MESSAGE(STATUS "###########################OS is windows###########################")
	
	include_directories(
		C:/boost_1_62_0
		${CMAKE_SOURCE_DIR}/lib/CTPForWin64
		${CMAKE_SOURCE_DIR}/lib/TalibForWin64/include
		${CMAKE_SOURCE_DIR}/lib/curl-7.63.0/libcurl-7.61.1-vc15-x64/include
		${CMAKE_SOURCE_DIR}/lib/openssl-1.1.1a/libForWin64/openssl-1.1.1a-vc15-x64/include
		${CMAKE_SOURCE_DIR}/lib/log4cplus/include
		${CMAKE_SOURCE_DIR}/thirdparty/DisruptorCpp
		${CMAKE_SOURCE_DIR}/thirdparty/fast-cpp-csv-parser
		${CMAKE_SOURCE_DIR}/thirdparty/cpr/include
		${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include
		)

	link_directories(
		C:/boost_1_62_0/lib64-msvc-14.0
		${CMAKE_SOURCE_DIR}/lib/CTPForWin64
		${CMAKE_SOURCE_DIR}/lib/TalibForWin64/lib
		${CMAKE_SOURCE_DIR}/lib/log4cplus/libForWin64
		${CMAKE_SOURCE_DIR}/lib/curl-7.63.0/libcurl-7.61.1-vc15-x64/lib
		${CMAKE_SOURCE_DIR}/lib/openssl-1.1.1a/libForWin64/openssl-1.1.1a-vc15-x64/lib
		)
		
ELSEIF (UNIX)
	MESSAGE(STATUS "###########################OS is UNIX-like OS's.###########################")
	
	include_directories(
		/opt/kungfu/toolchain/boost-1.62.0/include
		${CMAKE_SOURCE_DIR}/lib/CTPForLinux64
		${CMAKE_SOURCE_DIR}/lib/TalibForLinux64/include
		${CMAKE_SOURCE_DIR}/lib/curl-7.63.0/linux763/include
		${CMAKE_SOURCE_DIR}/lib/openssl-1.1.1a/libForLinux64/include
		${CMAKE_SOURCE_DIR}/lib/log4cplus/include
		${CMAKE_SOURCE_DIR}/thirdparty/DisruptorCpp
		${CMAKE_SOURCE_DIR}/thirdparty/fast-cpp-csv-parser
		${CMAKE_SOURCE_DIR}/thirdparty/cpr/include
		${CMAKE_SOURCE_DIR}/thirdparty/rapidjson/include
		)
	
	link_directories(
		/opt/kungfu/toolchain/boost-1.62.0/lib/
		${CMAKE_SOURCE_DIR}/lib/CTPForLinux64
		${CMAKE_SOURCE_DIR}/lib/TalibForLinux64/lib
		${CMAKE_SOURCE_DIR}/lib/log4cplus/libForLinux64
		${CMAKE_SOURCE_DIR}/lib/curl-7.63.0/linux763/libForLinux64
		${CMAKE_SOURCE_DIR}/lib/openssl-1.1.1a/libForLinux64
		)
		
ENDIF ()


#ADD_SUBDIRECTORY(thirdparty)

aux_source_directory("${CMAKE_SOURCE_DIR}/thirdparty/cpr/cpr/" CPR_SOURCE_FILES)
add_library(cpr STATIC ${CPR_SOURCE_FILES})

aux_source_directory("${CMAKE_SOURCE_DIR}/thirdparty/DisruptorCpp/Disruptor/" DisruptorCpp_SOURCE_FILES)
add_library(Disruptor STATIC ${DisruptorCpp_SOURCE_FILES})

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(TARGET cpr PROPERTY FOLDER "thirdparty")
SET_PROPERTY(TARGET Disruptor PROPERTY FOLDER "thirdparty")

SET(SOURCE_FILES_MAIN src/main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES_MAIN})
ADD_DEPENDENCIES(${PROJECT_NAME} cpr Disruptor)

IF (WIN32)
	MESSAGE(STATUS "###########################OS is windows###########################")
		
	target_link_libraries(${PROJECT_NAME}
			log4cplus.lib
			thostmduserapi.lib
			thosttraderapi.lib
			ta_abstract_cmr.lib
			ta_common_cmr.lib
			ta_func_cmr.lib
			ta_libc_cmr.lib
			libcurl.lib
			libcrypto.lib
			libssl.lib
			cpr
			Disruptor
			)
			
ELSEIF (UNIX)
	MESSAGE(STATUS "###########################OS is UNIX-like OS's.###########################")
	
		target_link_libraries(${PROJECT_NAME}
			pthread
			boost_thread
			boost_system
			liblog4cplus.so
			thostmduserapi.so
			thosttraderapi.so
			libta_abstract_cmr.a
			libta_common_cmr.a
			libta_func_cmr.a
			libta_libc_cmr.a
			libcurl.so
			libcrypto.so
			libssl.so
			cpr
			Disruptor
			)
ENDIF ()


#create VS2017 sln
#makedir build
#cd build		
#build>cmake .. -G "Visual Studio 15 2017 Win64"
