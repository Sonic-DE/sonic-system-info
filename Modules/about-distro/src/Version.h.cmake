/*
    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
    SPDX-FileCopyrightText: 2021-2021 Harald Sitter <sitter@kde.org>
*/

#ifndef VERSION_H
#define VERSION_H

// NOTE: we want the cmakedefines so undefined variables result in undefined macros result in build failures!
#cmakedefine PROJECT_VERSION "@PROJECT_VERSION@"

#endif // VERSION_H
