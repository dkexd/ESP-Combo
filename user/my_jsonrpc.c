#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"
#include "my_jsonrpc.h"




#define	MAXSIZE 4096

/* Error codes
enum error_code{
	Parse_error = -32700,
	Invalid_Request = -32600,
	Method_not_found = -32601,
  Invalid_params = -32602,
  Internal_error = -32603
};
*/
//-32700	Parse error	Invalid JSON was received by the server.
//An error occurred on the server while parsing the JSON text.
//-32600	Invalid Request	The JSON sent is not a valid Request object.
//-32601	Method not found	The method does not exist / is not available.
//-32602	Invalid params	Invalid method parameter(s).
//-32603	Internal error	Internal JSON-RPC error.



const char check_jsonrpc[]="\"2.0\"";
int struct_mark = 0;
//char *ptr;
//ptr = &buffer[0];
/*

static char * jsonrpc_resp_ver
static int jsonrpc_resp_err_code
static char * jsonrpc_resp_err_msg
static char * jsonrpc_resp_err_data
static char * jsonrpc_resp_res
static char * jsonrpc_req_meth
static char * jsonrpc_req_params
static char * jsonrpc_req_params_url
static char * jsonrpc_req_params_data
static char * jsonrpc_req_params_data_id
static char * jsonrpc_req_params_data_type
static char * jsonrpc_req_params_data_name
static char * jsonrpc_req_params_data_number
static char * jsonrpc_req_params_data_number_value1
static int jsonrpc_id


*/



//JSON strings
char my_json_string[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":    1920, \n\"height\":     1080, \n\"interlace\":  false,\n\"frame rate\": 24\n}\n}";
char sent[]="{\"jsonrpc\":\"2.0\",\"id\":\"1\",\"method\":\"POST\",\"params\":{\"url\":\"/network\",\"data\":{\":id\":\"bebb68a0-aaaa-11e6-86ca-001a65002cf0\",\":type\":\"urn:seluxit:xml:bastard:network-1.0\",\"name\":\"led\",\"number\":{\"value1\":\"543\"}}}}";
char pdata[]="{\"jsonrpc\":\"2.0\",\"id\": 1,\"result\":true}";
char pdata_err[]="{\"jsonrpc\":\"2.0\",\"id\": 1,\"error\": {\"code\": -32700, \"message\": \"Parse error\", \"data\": \"A disastrous encounter of an angry racoon!\"}}";
char testcrap[]="dgfuhjioklhkuyjhfgtf5y65y6";


//DEFINITION Creating JSON RPC Request structure
void jsonrpc_create(){
  //char * jsonrpc_send_ver;
	//jsonrpc_send_ver = "2.0";


	cJSON *root,*params,*data,*number;
	root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "jsonrpc", cJSON_CreateString("2.0"));
	cJSON_AddItemToObject(root, "id", cJSON_CreateNumber(1));
	cJSON_AddItemToObject(root, "method", cJSON_CreateString("POST"));
	cJSON_AddItemToObject(root, "params", params = cJSON_CreateObject());
		cJSON_AddStringToObject(params, "url", "/network/bebb68a0-aaaa-11e6-86ca-001a65002cf0/device/bebb68a0-bbbb-11e6-86ca-001a65002cf0/service");
		cJSON_AddItemToObject(params, "data", data = cJSON_CreateObject());
			cJSON_AddStringToObject(data, ":id", "1c727866-c5bb-45e6-977e-7a3828a4b526");
			cJSON_AddStringToObject(data, ":type", "led");
			cJSON_AddStringToObject(data, "permission", "rw");
			cJSON_AddStringToObject(data, "type", "on/off");
			cJSON_AddItemToObject(data, "number", number = cJSON_CreateObject());
				cJSON_AddNumberToObject(number, "min", 0);
				cJSON_AddNumberToObject(number, "max", 1);
				cJSON_AddNumberToObject(number, "step", 1);
			cJSON_AddStringToObject(data, "status", "ok");

	char *new_json_string=cJSON_Print(root);
	printf("%s\n", new_json_string);
	//CALLING function to parse newly created JSONRPC string
	jsonrpc_read_test(new_json_string);



}
//DESCRIPTION Parsing created JSON RPC Request to send string
void jsonrpc_read_test(char * new_json_string){

		cJSON * root = cJSON_Parse(new_json_string);
		cJSON * params = cJSON_GetObjectItem(root,"params");
		cJSON * data = cJSON_GetObjectItem(params,"data");
		cJSON * number = cJSON_GetObjectItem(data,"number");
	//	cJSON * max = cJSON_GetObjectItem(number,"max");
		int maxmax = cJSON_GetObjectItem(number,"max")->valueint;
		//printing out value of max variable insude the JSONRPC structure
		printf("root/params/data/number/max = %d\n",maxmax);

		cJSON_Delete(root);
}
//DESCRIPTION Parsing recieved JSON RPC Response
void jsonrpc_parse(const char* text) {


		printf("Pre-processed string looks like this: %s\n",text);
		cJSON * root = cJSON_Parse(text);

		if (root == NULL) {
			printf("No such element in JSON!\n");
			cJSON_Delete(root);
			printf("Exiting parsing function!\n");
			return;
		}

//VERSION
		cJSON * jsonrpc = cJSON_GetObjectItem(root,"jsonrpc");
		if (jsonrpc == NULL) {
			printf("No such element in root!\n");
			cJSON_Delete(root);
			printf("Exiting parsing function!\n");
			return;
		}
		printf("RECIEVED JSON RPC MESSAGE!\n");
		printf("--------------------------\n");

		char * jsonrpc_resp_ver = cJSON_Print(jsonrpc);
		printf("JSON RPC version: %s\n",jsonrpc_resp_ver);
		printf("--------------------\n");

//RESPONSE OR REQUEST

		cJSON * method = cJSON_GetObjectItem(root,"method");
		if (method == NULL) {
/*-------------------------------------------------------------------------------------------------------*/
			//START parsing RESPONSE
					printf("It's a RESPONSE!\n");
					printf("--------------------------\n");
				//Check if there result or error object
				cJSON * result = cJSON_GetObjectItem(root,"result");
				//if result is NULL, then check error
				if (result == NULL) {
					cJSON * error = cJSON_GetObjectItem(root,"error");

					if (error == NULL) {
							printf("No such element in root!\n");
							cJSON_Delete(root);
							printf("Exiting parsing function!\n");
							return;
						}
		   		//ERROR is good
					else{
						//marked as error
						struct_mark = 1;
						printf("OH NOES, it's an Error Response!\n");
						//open error objects here
						cJSON * code = cJSON_GetObjectItem(error,"code");
						if (code == NULL) {
								printf("No such element in error!\n");
								//cJSON_Delete(root);
								printf("Exiting parsing function!\n");
								return;
							}
						int jsonrpc_resp_err_code = cJSON_GetObjectItem(error,"code")->valueint;
						printf("JSON RPC Error code: %d\n",jsonrpc_resp_err_code);

						cJSON * message = cJSON_GetObjectItem(error,"message");
						if (message == NULL) {
								printf("No such element in error!\n");
								//cJSON_Delete(root);
								printf("Exiting parsing function!\n");
								return;
							}

					char * jsonrpc_resp_err_msg = cJSON_Print(message);
					/*
					switch (jsonrpc_resp_err_code) {
						case -32700:
							jsonrpc_resp_err_msg = "Parse error";
						case -32600:
							jsonrpc_resp_err_msg = "Invalid Request";
						case -32601:
							jsonrpc_resp_err_msg = "Method not found";
						case -32602:
							jsonrpc_resp_err_msg = "Invalid params";
						case -32603:
							jsonrpc_resp_err_msg = "Internal error";
						case -32099 ... -32000:
							jsonrpc_resp_err_msg = "Server error";
					}
					*/
						printf("JSON RPC Error message: %s\n",jsonrpc_resp_err_msg);

						cJSON * data = cJSON_GetObjectItem(error,"data");
						if (data == NULL) {
								printf("No such element in error!\n");
								//cJSON_Delete(root);
								printf("Exiting parsing function!\n");
								return;
							}
						char * jsonrpc_resp_err_data = cJSON_Print(data);
						printf("JSON RPC Error data: %s\n",jsonrpc_resp_err_data);
					}

				}
				//RESULT is OK
				else{
					//marked as no error
					  struct_mark = 0;
						char * jsonrpc_resp_res = cJSON_Print(result);
						printf("JSON RPC result: %s\n",jsonrpc_resp_res);
				}








//END of parsing RESPONSE
/*-----------------------------------------------------------------------------------------------------*/
		}



//START of parsing REQUEST
    else{
			printf("It's a REQUEST!\n");
			printf("--------------------------\n");
//METHOD
			cJSON * method = cJSON_GetObjectItem(root,"method");
			if (method == NULL) {
					printf("No such element in root!\n");
					//cJSON_Delete(root);
					printf("Exiting parsing function!\n");
					return;
				}
			else{
				char * jsonrpc_req_meth = cJSON_Print(method);
				printf("JSON RPC Method: %s\n",jsonrpc_req_meth);
			}
//PARAMS
			cJSON * params = cJSON_GetObjectItem(root,"params");
			if (params == NULL) {
					printf("No such element in root!\n");
					//cJSON_Delete(root);
					printf("Exiting parsing function!\n");
					return;
			}
			else{
				char * jsonrpc_req_params = cJSON_Print(params);
				//printf("JSON RPC Parameters: %s\n",jsonrpc_req_params);
			}
////params.url
				cJSON * url = cJSON_GetObjectItem(params,"url");
				if (url == NULL) {
						printf("No such element in params!\n");
						//cJSON_Delete(root);
						printf("Exiting parsing function!\n");
						return;
				}
				else{
					char * jsonrpc_req_params_url = cJSON_Print(url);
					printf("+JSON RPC url: %s\n",jsonrpc_req_params_url);
				}
////params.data
				cJSON * data = cJSON_GetObjectItem(params,"data");
				if (data == NULL) {
						printf("No such element in params!\n");
						//cJSON_Delete(root);
						printf("Exiting parsing function!\n");
						return;
				}
				else{
					char * jsonrpc_req_params_data = cJSON_Print(data);
					//printf("+JSON RPC data: %s\n",jsonrpc_req_params_data);
				}
//////params.data.:id
					cJSON * id = cJSON_GetObjectItem(data,":id");
					if (id == NULL) {
							printf("No such element in data!\n");
							//cJSON_Delete(root);
							//printf("Exiting parsing function!\n");
						}
					else{
						char * jsonrpc_req_params_data_id = cJSON_Print(id);
						printf("++JSON RPC :id: %s\n",jsonrpc_req_params_data_id);
					}
//////params.data.:type
					cJSON * type = cJSON_GetObjectItem(data,":type");
					if (type == NULL) {
							printf("No such element in data!\n");
							//cJSON_Delete(root);
							//printf("Exiting parsing function!\n");
						}
					else{
						char * jsonrpc_req_params_data_type = cJSON_Print(type);
						printf("++JSON RPC :type: %s\n",jsonrpc_req_params_data_type);
					}
//////params.data.name
					cJSON * name = cJSON_GetObjectItem(data,"name");
					if (name == NULL) {
							printf("No such element in data_name!\n");
							//cJSON_Delete(root);
							//printf("Exiting parsing function!\n");
						}
					else{
						char * jsonrpc_req_params_data_name = cJSON_Print(name);
						printf("++JSON RPC name: %s\n",jsonrpc_req_params_data_name);
					}
//////params.data.number
					cJSON * number = cJSON_GetObjectItem(data,"number");
					if (number == NULL) {
							printf("No such element in data_number!\n");
							//cJSON_Delete(root);
							//printf("Exiting parsing function!\n");
						}
					else{
						char * jsonrpc_req_params_data_number = cJSON_Print(number);
						//printf("++JSON RPC number: %s\n",jsonrpc_req_params_data_number);
////////params.data.number.value1
						cJSON * value1 = cJSON_GetObjectItem(number,"value1");
						if (value1 == NULL) {
								printf("No such element in number_value1!\n");
								//cJSON_Delete(root);
								//printf("Exiting parsing function!\n");
							}
						else{
							char * jsonrpc_req_params_data_number_value1 = cJSON_Print(value1);
							printf("+++JSON RPC value1: %s\n",jsonrpc_req_params_data_number_value1);
						}
					}



//END of parsing REQUEST
		}

//ID
		cJSON * id = cJSON_GetObjectItem(root,"id");
		if (id == NULL) {
			printf("No such element in root!\n");
			cJSON_Delete(root);
			printf("Exiting parsing function!\n");
			return;
		}
		else{
			int jsonrpc_id = cJSON_GetObjectItem(root,"id")->valueint;
			printf("JSON RPC id: %d\n",jsonrpc_id);
		}

		printf("--------------------------\n");
		cJSON_Delete(root);


//define Structure of JSONRPC Response
/*

--- case struct_mark = 1 then is response_result
--- case struct_mark = 2 then is response_error
--- case struct_mark = 3 then is request



typedef enum{
	RESULT_OUTPUT,
	RESULT_ERR,
} STATUS_TYPE;

static struct Jsonrpc_response_all{
		unsigned char id;
		char jsonrpc_resp_code;
		char jsonrpc_msg;
		char jsonspc_data;
}

	switch (struct_mark) {

		case 1:
			Jsonrpc_response_all.id = RESULT_OUTPUT
			Jsonrpc_response_all.resp_code =
			static struct Jsonrpc_response_result_output{
			    char jsonrpc_resp_res;
					char jsonrpc_id;
			}


		case 2:

			static struct Jsonrpc_response_error_output{
					struct Jsonrpc_Error{
						int	jsonrpc_resp_err_code;
						char jsonrpc_resp_err_msg;
						char jsonrpc_resp_err_data;
					}
					char jsonrpc_id;
			}


		case 3:

			static struct Jsonrpc_request_output{
					char method;
					struct Jsonrpc_Params{
						char jsonrpc_req_params_url;
						struct Jsonrpc_Data{
							char jsonrpc_req_params_data_id;
							char jsonrpc_req_params_data_type;
							char jsonrpc_req_params_data_name;
							struct Jsonrpc_Name{
								int jsonrpc_req_params_data_number_value1;
							}

						};
					char jsonrpc_id;
				}
		  }


	}
	*/
	//struct switch END
}
//jsonrpc_parse END
