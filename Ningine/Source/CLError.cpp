#include "CLError.hpp"

std::string getErrorString(cl_int errorCode)
{
  std::string errorString{};
  switch(errorCode)
  {
    // run-time and JIT compiler errors
    case 0: errorString = "CL_SUCCESS"; break;
    case -1: errorString = "CL_DEVICE_NOT_FOUND"; break;
    case -2: errorString = "CL_DEVICE_NOT_AVAILABLE"; break;
    case -3: errorString = "CL_COMPILER_NOT_AVAILABLE"; break;
    case -4: errorString = "CL_MEM_OBJECT_ALLOCATION_FAILURE"; break;
    case -5: errorString = "CL_OUT_OF_RESOURCES"; break;
    case -6: errorString = "CL_OUT_OF_HOST_MEMORY"; break;
    case -7: errorString = "CL_PROFILING_INFO_NOT_AVAILABLE"; break;
    case -8: errorString = "CL_MEM_COPY_OVERLAP"; break;
    case -9: errorString = "CL_IMAGE_FORMAT_MISMATCH"; break;
    case -10: errorString = "CL_IMAGE_FORMAT_NOT_SUPPORTED"; break;
    case -11: errorString = "CL_BUILD_PROGRAM_FAILURE"; break;
    case -12: errorString = "CL_MAP_FAILURE"; break;
    case -13: errorString = "CL_MISALIGNED_SUB_BUFFER_OFFSET"; break;
    case -14: errorString = "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"; break;
    case -15: errorString = "CL_COMPILE_PROGRAM_FAILURE"; break;
    case -16: errorString = "CL_LINKER_NOT_AVAILABLE"; break;
    case -17: errorString = "CL_LINK_PROGRAM_FAILURE"; break;
    case -18: errorString = "CL_DEVICE_PARTITION_FAILED"; break;
    case -19: errorString = "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"; break;

      // compile-time errors
    case -30: errorString = "CL_INVALID_VALUE"; break;
    case -31: errorString = "CL_INVALID_DEVICE_TYPE"; break;
    case -32: errorString = "CL_INVALID_PLATFORM"; break;
    case -33: errorString = "CL_INVALID_DEVICE"; break;
    case -34: errorString = "CL_INVALID_CONTEXT"; break;
    case -35: errorString = "CL_INVALID_QUEUE_PROPERTIES"; break;
    case -36: errorString = "CL_INVALID_COMMAND_QUEUE"; break;
    case -37: errorString = "CL_INVALID_HOST_PTR"; break;
    case -38: errorString = "CL_INVALID_MEM_OBJECT"; break;
    case -39: errorString = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"; break;
    case -40: errorString = "CL_INVALID_IMAGE_SIZE"; break;
    case -41: errorString = "CL_INVALID_SAMPLER"; break;
    case -42: errorString = "CL_INVALID_BINARY"; break;
    case -43: errorString = "CL_INVALID_BUILD_OPTIONS"; break;
    case -44: errorString = "CL_INVALID_PROGRAM"; break;
    case -45: errorString = "CL_INVALID_PROGRAM_EXECUTABLE"; break;
    case -46: errorString = "CL_INVALID_KERNEL_NAME"; break;
    case -47: errorString = "CL_INVALID_KERNEL_DEFINITION"; break;
    case -48: errorString = "CL_INVALID_KERNEL"; break;
    case -49: errorString = "CL_INVALID_ARG_INDEX"; break;
    case -50: errorString = "CL_INVALID_ARG_VALUE"; break;
    case -51: errorString = "CL_INVALID_ARG_SIZE"; break;
    case -52: errorString = "CL_INVALID_KERNEL_ARGS"; break;
    case -53: errorString = "CL_INVALID_WORK_DIMENSION"; break;
    case -54: errorString = "CL_INVALID_WORK_GROUP_SIZE"; break;
    case -55: errorString = "CL_INVALID_WORK_ITEM_SIZE"; break;
    case -56: errorString = "CL_INVALID_GLOBAL_OFFSET"; break;
    case -57: errorString = "CL_INVALID_EVENT_WAIT_LIST"; break;
    case -58: errorString = "CL_INVALID_EVENT"; break;
    case -59: errorString = "CL_INVALID_OPERATION"; break;
    case -60: errorString = "CL_INVALID_GL_OBJECT"; break;
    case -61: errorString = "CL_INVALID_BUFFER_SIZE"; break;
    case -62: errorString = "CL_INVALID_MIP_LEVEL"; break;
    case -63: errorString = "CL_INVALID_GLOBAL_WORK_SIZE"; break;
    case -64: errorString = "CL_INVALID_PROPERTY"; break;
    case -65: errorString = "CL_INVALID_IMAGE_DESCRIPTOR"; break;
    case -66: errorString = "CL_INVALID_COMPILER_OPTIONS"; break;
    case -67: errorString = "CL_INVALID_LINKER_OPTIONS"; break;
    case -68: errorString = "CL_INVALID_DEVICE_PARTITION_COUNT"; break;

      // extension errors
    case -1000: errorString = "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR"; break;
    case -1001: errorString = "CL_PLATFORM_NOT_FOUND_KHR"; break;
    case -1002: errorString = "CL_INVALID_D3D10_DEVICE_KHR"; break;
    case -1003: errorString = "CL_INVALID_D3D10_RESOURCE_KHR"; break;
    case -1004: errorString = "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR"; break;
    case -1005: errorString = "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR"; break;
    default: errorString = "Unknown OpenCL error(" + std::to_string(errorCode) + ")"; break;
  }

  return errorString;
}
