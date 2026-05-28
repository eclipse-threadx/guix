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



class widget_writer {
    public:
        static void WriteWidgets(xml_writer &writer, studiox_project *project, int display_index, widget_info *start, BOOL DoSiblings = TRUE);
        static void WriteWidgetFolders(xml_writer &writer, studiox_project *project, int display_index, folder_info *start, BOOL DoSiblings = TRUE);
};