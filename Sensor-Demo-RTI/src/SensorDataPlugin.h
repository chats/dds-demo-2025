

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from SensorData.idl
using RTI Code Generator (rtiddsgen) version 4.3.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef SensorDataPlugin_429345856_h
#define SensorDataPlugin_429345856_h

#include "SensorData.h"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
    #endif

    /* The type used to store keys for instances of type struct
    * AnotherSimple.
    *
    * By default, this type is struct Reading
    * itself. However, if for some reason this choice is not practical for your
    * system (e.g. if sizeof(struct Reading)
    * is very large), you may redefine this typedef in terms of another type of
    * your choosing. HOWEVER, if you define the KeyHolder type to be something
    * other than struct AnotherSimple, the
    * following restriction applies: the key of struct
    * Reading must consist of a
    * single field of your redefined KeyHolder type and that field must be the
    * first field in struct Reading.
    */
    typedef  struct SensorData_Reading SensorData_ReadingKeyHolder;

    #define SensorData_ReadingPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample 

    #define SensorData_ReadingPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
    #define SensorData_ReadingPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

    #define SensorData_ReadingPlugin_get_key PRESTypePluginDefaultEndpointData_getKey 
    #define SensorData_ReadingPlugin_return_key PRESTypePluginDefaultEndpointData_returnKey

    #define SensorData_ReadingPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
    #define SensorData_ReadingPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

    /* --------------------------------------------------------------------------------------
    Support functions:
    * -------------------------------------------------------------------------------------- */

    NDDSUSERDllExport extern SensorData_Reading*
    SensorData_ReadingPluginSupport_create_data_w_params(
        const struct DDS_TypeAllocationParams_t * alloc_params);

    NDDSUSERDllExport extern SensorData_Reading*
    SensorData_ReadingPluginSupport_create_data_ex(RTIBool allocate_pointers);

    NDDSUSERDllExport extern SensorData_Reading*
    SensorData_ReadingPluginSupport_create_data(void);

    NDDSUSERDllExport extern RTIBool 
    SensorData_ReadingPluginSupport_copy_data(
        SensorData_Reading *out,
        const SensorData_Reading *in);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPluginSupport_destroy_data_w_params(
        SensorData_Reading *sample,
        const struct DDS_TypeDeallocationParams_t * dealloc_params);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPluginSupport_destroy_data_ex(
        SensorData_Reading *sample,RTIBool deallocate_pointers);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPluginSupport_destroy_data(
        SensorData_Reading *sample);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPluginSupport_print_data(
        const SensorData_Reading *sample,
        const char *desc,
        unsigned int indent);

    NDDSUSERDllExport extern SensorData_Reading*
    SensorData_ReadingPluginSupport_create_key_ex(RTIBool allocate_pointers);

    NDDSUSERDllExport extern SensorData_Reading*
    SensorData_ReadingPluginSupport_create_key(void);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPluginSupport_destroy_key_ex(
        SensorData_ReadingKeyHolder *key,RTIBool deallocate_pointers);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPluginSupport_destroy_key(
        SensorData_ReadingKeyHolder *key);

    /* ----------------------------------------------------------------------------
    Callback functions:
    * ---------------------------------------------------------------------------- */

    NDDSUSERDllExport extern PRESTypePluginParticipantData 
    SensorData_ReadingPlugin_on_participant_attached(
        void *registration_data, 
        const struct PRESTypePluginParticipantInfo *participant_info,
        RTIBool top_level_registration, 
        void *container_plugin_context,
        RTICdrTypeCode *typeCode);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPlugin_on_participant_detached(
        PRESTypePluginParticipantData participant_data);

    NDDSUSERDllExport extern PRESTypePluginEndpointData 
    SensorData_ReadingPlugin_on_endpoint_attached(
        PRESTypePluginParticipantData participant_data,
        const struct PRESTypePluginEndpointInfo *endpoint_info,
        RTIBool top_level_registration, 
        void *container_plugin_context);

    NDDSUSERDllExport extern void 
    SensorData_ReadingPlugin_on_endpoint_detached(
        PRESTypePluginEndpointData endpoint_data);

    NDDSUSERDllExport extern void    
    SensorData_ReadingPlugin_return_sample(
        PRESTypePluginEndpointData endpoint_data,
        SensorData_Reading *sample,
        void *handle);    

    NDDSUSERDllExport extern RTIBool 
    SensorData_ReadingPlugin_copy_sample(
        PRESTypePluginEndpointData endpoint_data,
        SensorData_Reading *out,
        const SensorData_Reading *in);

    /* ----------------------------------------------------------------------------
    (De)Serialize functions:
    * ------------------------------------------------------------------------- */

    NDDSUSERDllExport extern RTIBool
    SensorData_ReadingPlugin_serialize_to_cdr_buffer(
        char * buffer,
        unsigned int * length,
        const SensorData_Reading *sample); 

    NDDSUSERDllExport extern RTIBool
    SensorData_ReadingPlugin_serialize_to_cdr_buffer_ex(
        char *buffer,
        unsigned int *length,
        const SensorData_Reading *sample,
        DDS_DataRepresentationId_t representation);

    NDDSUSERDllExport extern RTIBool
    SensorData_ReadingPlugin_deserialize_from_cdr_buffer(
        SensorData_Reading *sample,
        const char * buffer,
        unsigned int length);    
    #if !defined (NDDS_STANDALONE_TYPE)
    NDDSUSERDllExport extern DDS_ReturnCode_t
    SensorData_ReadingPlugin_data_to_string(
        const SensorData_Reading *sample,
        char *str,
        DDS_UnsignedLong *str_size, 
        const struct DDS_PrintFormatProperty *property);    
    #endif

    NDDSUSERDllExport extern unsigned int 
    SensorData_ReadingPlugin_get_serialized_sample_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    /* --------------------------------------------------------------------------------------
    Key Management functions:
    * -------------------------------------------------------------------------------------- */
    NDDSUSERDllExport extern PRESTypePluginKeyKind 
    SensorData_ReadingPlugin_get_key_kind(void);

    NDDSUSERDllExport extern unsigned int 
    SensorData_ReadingPlugin_get_serialized_key_max_size(
        PRESTypePluginEndpointData endpoint_data,
        RTIBool include_encapsulation,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern unsigned int 
    SensorData_ReadingPlugin_get_serialized_key_max_size_for_keyhash(
        PRESTypePluginEndpointData endpoint_data,
        RTIEncapsulationId encapsulation_id,
        unsigned int current_alignment);

    NDDSUSERDllExport extern RTIBool 
    SensorData_ReadingPlugin_deserialize_key(
        PRESTypePluginEndpointData endpoint_data,
        SensorData_Reading ** sample,
        RTIBool * drop_sample,
        struct RTICdrStream *cdrStream,
        RTIBool deserialize_encapsulation,
        RTIBool deserialize_key,
        void *endpoint_plugin_qos);

    NDDSUSERDllExport extern RTIBool 
    SensorData_ReadingPlugin_instance_to_key(
        PRESTypePluginEndpointData endpoint_data,
        SensorData_ReadingKeyHolder *key, 
        const SensorData_Reading *instance);

    NDDSUSERDllExport extern RTIBool 
    SensorData_ReadingPlugin_key_to_instance(
        PRESTypePluginEndpointData endpoint_data,
        SensorData_Reading *instance, 
        const SensorData_ReadingKeyHolder *key);

    NDDSUSERDllExport extern RTIBool 
    SensorData_ReadingPlugin_serialized_sample_to_keyhash(
        PRESTypePluginEndpointData endpoint_data,
        struct RTICdrStream *cdrStream, 
        DDS_KeyHash_t *keyhash,
        RTIBool deserialize_encapsulation,
        void *endpoint_plugin_qos); 

    NDDSUSERDllExport extern
    struct RTIXCdrInterpreterPrograms * SensorData_ReadingPlugin_get_programs(void);

    /* Plugin Functions */
    NDDSUSERDllExport extern struct PRESTypePlugin*
    SensorData_ReadingPlugin_new(void);

    NDDSUSERDllExport extern void
    SensorData_ReadingPlugin_delete(struct PRESTypePlugin *);

    #ifdef __cplusplus
}
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* SensorDataPlugin_429345856_h */

