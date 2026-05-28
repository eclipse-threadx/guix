/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 Eclipse ThreadX contributors
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/* Logic to run the application in a standalone window */


class app_runner
{
    public:
        app_runner();
        ~app_runner();
        void RunApplication(int display_index, CWnd *parent = NULL);

    private:
        BOOL HaveStartupScreens(const CArray<widget_info *> *screen_list) const;
};
