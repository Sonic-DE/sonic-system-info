/*
 * SPDX-FileCopyrightText: 2026 Alexander Wilms <f.alexander.wilms@outlook.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include "Entry.h"
#include <vector>

class NetworkEntryFactory
{
public:
    /**
     * Creates Entry objects for all network adapters
     * @return List of network adapter entries for display
     */
    static std::vector<Entry *> factorize();
};
