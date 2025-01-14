

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from SensorData.idl 
using RTI Code Generator (rtiddsgen) version 4.3.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_c_h
#include "ndds/ndds_c.h"
#endif

#ifndef dds_c_log_infrastructure_h
#include "dds_c/dds_c_infrastructure_impl.h"       
#endif 

#ifndef cdr_type_h
#include "cdr/cdr_type.h"
#endif    

#ifndef osapi_heap_h
#include "osapi/osapi_heap.h" 
#endif
#else
#include "ndds_standalone_type.h"
#endif

#include "SensorData.h"

#ifndef NDDS_STANDALONE_TYPE
#include "SensorDataPlugin.h"
#endif

/* ========================================================================= */
const char *SensorData_ReadingTYPENAME = "SensorData::Reading";

#ifndef NDDS_STANDALONE_TYPE
DDS_TypeCode * SensorData_Reading_get_typecode(void)
{
    static RTIBool is_initialized = RTI_FALSE;

    static DDS_TypeCode SensorData_Reading_g_tc_device_id_string = DDS_INITIALIZE_STRING_TYPECODE((255L));

    static DDS_TypeCode_Member SensorData_Reading_g_tc_members[4]=
    {

        {
            (char *)"device_id",/* Member name */
            {
                0,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_KEY_MEMBER , /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER
        }, 
        {
            (char *)"temperature",/* Member name */
            {
                1,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER
        }, 
        {
            (char *)"humidity",/* Member name */
            {
                2,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER
        }, 
        {
            (char *)"timestamp",/* Member name */
            {
                3,/* Representation ID */
                DDS_BOOLEAN_FALSE,/* Is a pointer? */
                -1, /* Bitfield bits */
                NULL/* Member type code is assigned later */
            },
            0, /* Ignored */
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
            DDS_PUBLIC_MEMBER,/* Member visibility */
            1,
            NULL, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER
        }
    };

    static DDS_TypeCode SensorData_Reading_g_tc =
    {{
            DDS_TK_STRUCT, /* Kind */
            DDS_BOOLEAN_FALSE, /* Ignored */
            -1, /*Ignored*/
            (char *)"SensorData::Reading", /* Name */
            NULL, /* Ignored */      
            0, /* Ignored */
            0, /* Ignored */
            NULL, /* Ignored */
            4, /* Number of members */
            SensorData_Reading_g_tc_members, /* Members */
            DDS_VM_NONE, /* Ignored */
            RTICdrTypeCodeAnnotations_INITIALIZER,
            DDS_BOOLEAN_TRUE, /* _isCopyable */
            NULL, /* _sampleAccessInfo: assigned later */
            NULL /* _typePlugin: assigned later */
        }}; /* Type code for SensorData_Reading*/

    if (is_initialized) {
        return &SensorData_Reading_g_tc;
    }

    is_initialized = RTI_TRUE;

    SensorData_Reading_g_tc._data._annotations._allowedDataRepresentationMask = 5;

    SensorData_Reading_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&SensorData_Reading_g_tc_device_id_string;
    SensorData_Reading_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_double;
    SensorData_Reading_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_double;
    SensorData_Reading_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

    /* Initialize the values for member annotations. */
    SensorData_Reading_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
    SensorData_Reading_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
    SensorData_Reading_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_DOUBLE;
    SensorData_Reading_g_tc_members[1]._annotations._defaultValue._u.double_value = 0.0;
    SensorData_Reading_g_tc_members[1]._annotations._minValue._d = RTI_XCDR_TK_DOUBLE;
    SensorData_Reading_g_tc_members[1]._annotations._minValue._u.double_value = RTIXCdrDouble_MIN;
    SensorData_Reading_g_tc_members[1]._annotations._maxValue._d = RTI_XCDR_TK_DOUBLE;
    SensorData_Reading_g_tc_members[1]._annotations._maxValue._u.double_value = RTIXCdrDouble_MAX;
    SensorData_Reading_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_DOUBLE;
    SensorData_Reading_g_tc_members[2]._annotations._defaultValue._u.double_value = 0.0;
    SensorData_Reading_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_DOUBLE;
    SensorData_Reading_g_tc_members[2]._annotations._minValue._u.double_value = RTIXCdrDouble_MIN;
    SensorData_Reading_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_DOUBLE;
    SensorData_Reading_g_tc_members[2]._annotations._maxValue._u.double_value = RTIXCdrDouble_MAX;
    SensorData_Reading_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
    SensorData_Reading_g_tc_members[3]._annotations._defaultValue._u.long_value = 0;
    SensorData_Reading_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_LONG;
    SensorData_Reading_g_tc_members[3]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
    SensorData_Reading_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
    SensorData_Reading_g_tc_members[3]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;

    SensorData_Reading_g_tc._data._sampleAccessInfo =
    SensorData_Reading_get_sample_access_info();
    SensorData_Reading_g_tc._data._typePlugin =
    SensorData_Reading_get_type_plugin_info();    

    return &SensorData_Reading_g_tc;
}

RTIXCdrSampleAccessInfo *SensorData_Reading_get_sample_access_info()
{
    static RTIBool is_initialized = RTI_FALSE;

    static RTIXCdrMemberAccessInfo SensorData_Reading_g_memberAccessInfos[4] =
    {RTIXCdrMemberAccessInfo_INITIALIZER};

    static RTIXCdrSampleAccessInfo SensorData_Reading_g_sampleAccessInfo = 
    RTIXCdrSampleAccessInfo_INITIALIZER;

    if (is_initialized) {
        return (RTIXCdrSampleAccessInfo*) &SensorData_Reading_g_sampleAccessInfo;
    }

    SensorData_Reading_g_memberAccessInfos[0].bindingMemberValueOffset[0] =
    offsetof(struct SensorData_Reading, device_id);

    SensorData_Reading_g_memberAccessInfos[1].bindingMemberValueOffset[0] =
    offsetof(struct SensorData_Reading, temperature);

    SensorData_Reading_g_memberAccessInfos[2].bindingMemberValueOffset[0] =
    offsetof(struct SensorData_Reading, humidity);

    SensorData_Reading_g_memberAccessInfos[3].bindingMemberValueOffset[0] =
    offsetof(struct SensorData_Reading, timestamp);

    SensorData_Reading_g_sampleAccessInfo.memberAccessInfos = 
    SensorData_Reading_g_memberAccessInfos;

    {
        size_t candidateTypeSize = sizeof(SensorData_Reading);

        if (candidateTypeSize > RTIXCdrLong_MAX) {
            SensorData_Reading_g_sampleAccessInfo.typeSize[0] =
            RTIXCdrLong_MAX;
        } else {
            SensorData_Reading_g_sampleAccessInfo.typeSize[0] =
            (RTIXCdrUnsignedLong) candidateTypeSize;
        }
    }

    SensorData_Reading_g_sampleAccessInfo.languageBinding = 
    RTI_XCDR_TYPE_BINDING_C ;

    is_initialized = RTI_TRUE;
    return (RTIXCdrSampleAccessInfo*) &SensorData_Reading_g_sampleAccessInfo;
}
RTIXCdrTypePlugin *SensorData_Reading_get_type_plugin_info()
{
    static RTIXCdrTypePlugin SensorData_Reading_g_typePlugin = 
    {
        NULL, /* serialize */
        NULL, /* serialize_key */
        NULL, /* deserialize_sample */
        NULL, /* deserialize_key_sample */
        NULL, /* skip */
        NULL, /* get_serialized_sample_size */
        NULL, /* get_serialized_sample_max_size_ex */
        NULL, /* get_serialized_key_max_size_ex */
        NULL, /* get_serialized_sample_min_size */
        NULL, /* serialized_sample_to_key */
        (RTIXCdrTypePluginInitializeSampleFunction) 
        SensorData_Reading_initialize_ex,
        NULL,
        (RTIXCdrTypePluginFinalizeSampleFunction)
        SensorData_Reading_finalize_w_return,
        NULL,
        NULL
    };

    return &SensorData_Reading_g_typePlugin;
}
#endif

RTIBool SensorData_Reading_initialize(
    SensorData_Reading* sample)
{
    return SensorData_Reading_initialize_ex(
        sample, 
        RTI_TRUE, 
        RTI_TRUE);
}
RTIBool SensorData_Reading_initialize_w_params(
    SensorData_Reading *sample,
    const struct DDS_TypeAllocationParams_t *allocParams)
{

    if (sample == NULL) {
        return RTI_FALSE;
    }
    if (allocParams == NULL) {
        return RTI_FALSE;
    }

    if (allocParams->allocate_memory) {
        sample->device_id = DDS_String_alloc((255L));
        if (sample->device_id != NULL) {
            RTIOsapiUtility_unusedReturnValue(
                RTICdrType_copyStringEx(
                    &sample->device_id,
                    "",
                    (255L),
                    RTI_FALSE),
                    RTIBool);
        }
        if (sample->device_id == NULL) {
            return RTI_FALSE;
        }
    } else {
        if (sample->device_id != NULL) {
            RTIOsapiUtility_unusedReturnValue(
                RTICdrType_copyStringEx(
                    &sample->device_id,
                    "",
                    (255L),
                    RTI_FALSE),
                    RTIBool);
            if (sample->device_id == NULL) {
                return RTI_FALSE;
            }
        }
    }

    sample->temperature = 0.0;

    sample->humidity = 0.0;

    sample->timestamp = 0;

    return RTI_TRUE;
}
RTIBool SensorData_Reading_initialize_ex(
    SensorData_Reading *sample,
    RTIBool allocatePointers, 
    RTIBool allocateMemory)
{

    struct DDS_TypeAllocationParams_t allocParams =
    DDS_TYPE_ALLOCATION_PARAMS_DEFAULT;

    allocParams.allocate_pointers =  (DDS_Boolean)allocatePointers;
    allocParams.allocate_memory = (DDS_Boolean)allocateMemory;

    return SensorData_Reading_initialize_w_params(
        sample,
        &allocParams);
}

RTIBool SensorData_Reading_finalize_w_return(
    SensorData_Reading* sample)
{
    SensorData_Reading_finalize_ex(sample, RTI_TRUE);

    return RTI_TRUE;
}

void SensorData_Reading_finalize(
    SensorData_Reading* sample)
{  
    SensorData_Reading_finalize_ex(
        sample, 
        RTI_TRUE);
}

void SensorData_Reading_finalize_ex(
    SensorData_Reading *sample,
    RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParams =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;

    if (sample==NULL) {
        return;
    } 

    deallocParams.delete_pointers = (DDS_Boolean)deletePointers;

    SensorData_Reading_finalize_w_params(
        sample,
        &deallocParams);
}

void SensorData_Reading_finalize_w_params(
    SensorData_Reading *sample,
    const struct DDS_TypeDeallocationParams_t *deallocParams)
{
    if (sample==NULL) {
        return;
    }

    if (deallocParams == NULL) {
        return;
    }

    if (sample->device_id != NULL) {
        DDS_String_free(sample->device_id);
        sample->device_id=NULL;

    }

}

void SensorData_Reading_finalize_optional_members(
    SensorData_Reading* sample, RTIBool deletePointers)
{
    struct DDS_TypeDeallocationParams_t deallocParamsTmp =
    DDS_TYPE_DEALLOCATION_PARAMS_DEFAULT;
    struct DDS_TypeDeallocationParams_t * deallocParams =
    &deallocParamsTmp;

    if (sample==NULL) {
        return;
    } 
    if (deallocParams) {} /* To avoid warnings */

    deallocParamsTmp.delete_pointers = (DDS_Boolean)deletePointers;
    deallocParamsTmp.delete_optional_members = DDS_BOOLEAN_TRUE;

}

RTIBool SensorData_Reading_copy(
    SensorData_Reading* dst,
    const SensorData_Reading* src)
{

    if (dst == NULL || src == NULL) {
        return RTI_FALSE;
    }

    if (!RTICdrType_copyStringEx (
        &dst->device_id
        ,
        src->device_id, 
        (255L) + 1,
        RTI_FALSE)){
        return RTI_FALSE;
    }
    if (!RTICdrType_copyDouble (
        &dst->temperature, 
        &src->temperature)) { 
        return RTI_FALSE;
    }
    if (!RTICdrType_copyDouble (
        &dst->humidity, 
        &src->humidity)) { 
        return RTI_FALSE;
    }
    if (!RTICdrType_copyLong (
        &dst->timestamp, 
        &src->timestamp)) { 
        return RTI_FALSE;
    }

    return RTI_TRUE;
}

/**
* <<IMPLEMENTATION>>
*
* Defines:  TSeq, T
*
* Configure and implement 'SensorData_Reading' sequence class.
*/
#define T SensorData_Reading
#define TSeq SensorData_ReadingSeq

#define T_initialize_w_params SensorData_Reading_initialize_w_params

#define T_finalize_w_params   SensorData_Reading_finalize_w_params
#define T_copy       SensorData_Reading_copy

#ifndef NDDS_STANDALONE_TYPE
#include "dds_c/generic/dds_c_sequence_TSeq.gen"
#else
#include "dds_c_sequence_TSeq.gen"
#endif

#undef T_copy
#undef T_finalize_w_params

#undef T_initialize_w_params

#undef TSeq
#undef T

