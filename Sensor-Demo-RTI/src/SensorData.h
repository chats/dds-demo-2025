

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from SensorData.idl
using RTI Code Generator (rtiddsgen) version 4.3.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef SensorData_429345856_h
#define SensorData_429345856_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_c_h
#include "ndds/ndds_c.h"
#endif
#else
#include "ndds_standalone_type.h"
#endif

#ifdef __cplusplus
extern "C" {
    #endif

    extern const char *SensorData_ReadingTYPENAME;

    typedef struct SensorData_Reading
    {

        DDS_Char * device_id;
        DDS_Double temperature;
        DDS_Double humidity;
        DDS_Long timestamp;

    } SensorData_Reading ;
    #if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
    /* If the code is building on Windows, start exporting symbols.
    */
    #undef NDDSUSERDllExport
    #define NDDSUSERDllExport __declspec(dllexport)
    #endif

    #ifndef NDDS_STANDALONE_TYPE
    NDDSUSERDllExport DDS_TypeCode * SensorData_Reading_get_typecode(void); /* Type code */
    NDDSUSERDllExport RTIXCdrTypePlugin *SensorData_Reading_get_type_plugin_info(void);
    NDDSUSERDllExport RTIXCdrSampleAccessInfo *SensorData_Reading_get_sample_access_info(void);
    #endif
    DDS_SEQUENCE(SensorData_ReadingSeq, SensorData_Reading);

    NDDSUSERDllExport
    RTIBool SensorData_Reading_initialize(
        SensorData_Reading* self);

    NDDSUSERDllExport
    RTIBool SensorData_Reading_initialize_ex(
        SensorData_Reading* self,RTIBool allocatePointers,RTIBool allocateMemory);

    NDDSUSERDllExport
    RTIBool SensorData_Reading_initialize_w_params(
        SensorData_Reading* self,
        const struct DDS_TypeAllocationParams_t * allocParams);  

    NDDSUSERDllExport
    RTIBool SensorData_Reading_finalize_w_return(
        SensorData_Reading* self);

    NDDSUSERDllExport
    void SensorData_Reading_finalize(
        SensorData_Reading* self);

    NDDSUSERDllExport
    void SensorData_Reading_finalize_ex(
        SensorData_Reading* self,RTIBool deletePointers);

    NDDSUSERDllExport
    void SensorData_Reading_finalize_w_params(
        SensorData_Reading* self,
        const struct DDS_TypeDeallocationParams_t * deallocParams);

    NDDSUSERDllExport
    void SensorData_Reading_finalize_optional_members(
        SensorData_Reading* self, RTIBool deletePointers);  

    NDDSUSERDllExport
    RTIBool SensorData_Reading_copy(
        SensorData_Reading* dst,
        const SensorData_Reading* src);

    #if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
    /* If the code is building on Windows, stop exporting symbols.
    */
    #undef NDDSUSERDllExport
    #define NDDSUSERDllExport
    #endif

    #ifdef __cplusplus
}
#endif

#endif /* SensorData */

