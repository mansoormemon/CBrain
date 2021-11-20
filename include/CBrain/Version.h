// Copyright 2021 CBrain
// Project URL: https://github.com/mansoormemon/CBrain
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Copyright (c) 2021 Mansoor Memon <mansoorahmed.one@gmail.com>

#ifndef CBRAIN__VERSION_H_
#define CBRAIN__VERSION_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file Version.h
 *
 * @brief Contains version info of the library.
 *
 * @see
 * `https://semver.org/`
 *
 * @since v0.0.1.
 */

/**
 * @brief Library macro.
 */
#define CBRAIN

/**
 * @brief Library macro.
 */
#define __CBRAIN__

/**
 * @brief Library version.
 */
#define CBRAIN_VERSION 1

/**
 * @brief Library major version.
 */
#define CBRAIN_VERSION_MAJOR (CBRAIN_VERSION / 100000)

/**
 * @brief Library minor version.
 */
#define CBRAIN_VERSION_MINOR (CBRAIN_VERSION / 100 % 1000)

/**
 * @brief Library patch version.
 */
#define CBRAIN_VERSION_PATCH (CBRAIN_VERSION % 100)

#ifdef __cplusplus
}
#endif

#endif
