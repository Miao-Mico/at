/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include "at_device_template.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This function will initialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

errno_t at_windows_control_mount(struct at_device_package_s *package,
								 void *arg_list);

 /**
  * @brief This function will deinitialize the i/o stream of the windows.
  *
  * @param void
  *
  * @return void
  */

errno_t at_windows_control_demount(struct at_device_package_s *package);

/**
 * @brief This function will control the send/output of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

errno_t at_windows_control_send(void *device, void *data, size_t len);

/**
 * @brief This function will control the receive/input of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

void *at_windows_control_receive(void *device, size_t len);

/**
 * @brief This function will verify if the windows i/o device is valid.
 *
 * @param void
 *
 * @return void
 */

bool at_windows_control_verify_device(void *device);

/**
 * @brief This function will verify if the windows i/o package is valid.
 *
 * @param void
 *
 * @return void
 */

bool at_windows_control_verify_package(struct at_device_package_s *package);

#endif // _WIN32

/*
*********************************************************************************************************
*					LOCAL GLOBAL VARIABLES & LOCAL FUNCTION PROTOTYPES INTERSECTION
*********************************************************************************************************
*/

/**
 * @brief This struct will pack the device package to manage input/output in different platform.
 */

struct at_device_package_packer_s at_device_package_packer = {
	#ifdef _WIN32

	.windows_file_stream = at_device_package_packer_windows_file_stream,

	#endif // _WIN32
};

/*
*********************************************************************************************************
*                                            FUNCTIONS
*********************************************************************************************************
*/

#ifdef _WIN32

/**
 * @brief This function will package the operator of the windows file i/o stream into the package.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_device_package_packer_windows_file_stream(struct at_device_package_s **package,
											 void *arg_list)
{
	assert(package);

	if (at_device_control_configuration_init_package(package)) {
		return -1;
	}

	if (at_windows_control_mount((*package), arg_list)) {
		return 1;
	};

	if (!at_windows_control_verify_device((*package)->device_ptr)) {
		return 2;
	}

	(*package)->device_pack_id = AT_DEVICE_TEMPLATE_CFG_WINDOWS_FILE_STREAM_ID;

	(*package)->configuration.mount = (at_import_func_t)at_windows_control_mount;
	(*package)->configuration.demount = (at_import_func_t)at_windows_control_demount;

	(*package)->transmit.send = (at_import_func_t)at_windows_control_send;
	(*package)->transmit.receive = (at_import_func_t)at_windows_control_receive;

	(*package)->verify.device = (at_import_func_t)at_windows_control_verify_device;
	(*package)->verify.package = (at_import_func_t)at_windows_control_verify_package;

	if (!at_windows_control_verify_package((*package))) {
		free((*package));

		(*package) = NULL;
		return 3;
	}

	return 0;
}

/**
 * @brief This function will initialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_windows_control_mount(struct at_device_package_s *package,
						 void *arg_list)
{
	assert(package);
	assert(arg_list);

	errno_t err = 0;

	FILE *file = calloc(2, sizeof(FILE));

	if (NULL == file) {
		return -1;
	}

	if (err = fopen_s(((FILE **)file + 0), *((char **)arg_list + 0), *((char **)arg_list + 1))) {
		return err;
	}

	if (err = fopen_s(((FILE **)file + 1), *((char **)arg_list + 2), *((char **)arg_list + 3))) {
		return err;
	}

	if (!at_windows_control_verify_device(file)) {
		return 200;
	}

	package->device_ptr = file;

	return 0;
}

/**
 * @brief This function will deinitialize the i/o stream of the windows.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_windows_control_demount(struct at_device_package_s *package)
{
	assert(package);

	fclose(*((void **)package->device_ptr + 0));
	fclose(*((void **)package->device_ptr + 1));

	package->device_ptr = NULL;

	return 0;
}

/**
 * @brief This function will control the send/output of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

static inline errno_t
at_windows_control_send(void *device, void *data, size_t len)
{
	fwrite(data, sizeof(char), len, *((void **)device + 0));

	return 0;
}

/**
 * @brief This function will control the receive/input of the windows i/o.
 *
 * @param void
 *
 * @return void
 */

static inline void
*at_windows_control_receive(void *device, size_t len)
{
	char *string = calloc(1, sizeof(char) * (len + 1));

	if (NULL == string) {
		return NULL;
	}

	fgets(string, len, *((void **)device + 1));

	return string;
}

/**
 * @brief This function will verify if the windows i/o device is valid.
 *
 * @param void
 *
 * @return void
 */

static inline bool
at_windows_control_verify_device(void *device)
{
	assert(device);

	if (NULL == *((void **)device + 0) ||       /* Verify if the transmit part of the windows i/o package is valid */
		NULL == *((void **)device + 1)) {
		goto FAIL;
	}

	return true;

FAIL:
	return false;
}

/**
 * @brief This function will verify if the windows i/o package is valid.
 *
 * @param void
 *
 * @return void
 */

static inline bool
at_windows_control_verify_package(struct at_device_package_s *package)
{
	assert(package);

	if (NULL == package->transmit.send ||       /* Verify if the transmit part of the windows i/o package is valid */
		NULL == package->transmit.receive) {
		goto FAIL;
	}

	return true;

FAIL:
	return false;
}

#endif // _WIN32