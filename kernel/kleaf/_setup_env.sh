# SPDX-License-Identifier: GPL-2.0
#!/bin/bash

export ROOT_DIR=$(readlink -f $PWD)
export BUILD_CONFIG=${BUILD_CONFIG:-build.config}
KERNEL_VERSION_NUM=${DEVICE_MODULES_DIR##kernel_device_modules-}
KERNEL_VERSION=kernel-${KERNEL_VERSION_NUM}

echo ROOT_DIR: ${ROOT_DIR}
echo DEVICE_MODULES_DIR: ${DEVICE_MODULES_DIR}
echo KERNEL_VERSION: ${KERNEL_VERSION}

set -a
. ${ROOT_DIR}/${BUILD_CONFIG}
for fragment in ${BUILD_CONFIG_FRAGMENTS}; do
  . ${ROOT_DIR}/${fragment}
done
set +a

KLEAF_SUPPORTED_PROJECTS="mgk_64_k61"
KLEAF_GKI_CHECKER="no"

if [ -z ${MODE} ]
then
  MODE=user
fi

if [ -z ${OUT_DIR} ]
then
  OUT_DIR=${ROOT_DIR}/out
fi
if [ ! -d "${OUT_DIR}" ]; then
  OUT_DIR="${ROOT_DIR}/${OUT_DIR}"
fi

if [ "x${DEBUG}" == "x1" ]
then
DEBUG_ARGS="--sandbox_debug --verbose_failures"
fi
if [ -z ${SANDBOX} ]
then
  SANDBOX=1
else
  if [ "x${SANDBOX}" == "x0" ]
  then
    SANDBOX_ARGS="--config=local"
  fi
fi

BAZEL_EXPORT_ENV="BAZEL_DO_NOT_DETECT_CPP_TOOLCHAIN=1 \
	DEFCONFIG_OVERLAYS=${DEFCONFIG_OVERLAYS} \
	KERNEL_VERSION=${KERNEL_VERSION}"

KLEAF_GKI_CHECKER_COMMANDS=("python3 ${DEVICE_MODULES_DIR}/scripts/gki_checker.py -k ${KERNEL_VERSION} \
          -g ${ROOT_DIR}/../vendor/aosp_gki/${KERNEL_VERSION}/aarch64/vmlinux-userdebug \
          -w ${DEVICE_MODULES_DIR}/scripts/gki_checker_white_list.txt -O ${OUT_DIR}/check_gki")
