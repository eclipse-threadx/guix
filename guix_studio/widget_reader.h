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



class widget_reader {
    public:
        static widget_info *ReadOneWidget(xml_reader &reader, studiox_project *project, int display);
        static void ReadChildWidgets(xml_reader &reader, studiox_project *project, int display, widget_info *parent);
        static void ReadWidgets(xml_reader &reader, studiox_project *project, int display, folder_info *widgets_folder);
        static void ReadWidgetFolders(xml_reader &reader, studiox_project *project, int display);
};
