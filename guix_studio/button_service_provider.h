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

#ifndef _BUTTON_SERVICE_PROVIDER_
#define _BUTTON_SERVICE_PROVIDER_

class button_service_provider : public widget_service_provider
{
    public:
        button_service_provider();

        virtual int GetType() {return GX_TYPE_BUTTON;}
        virtual CString GetTypeName(widget_info *) {return CString("GX_TYPE_BUTTON");}
        virtual CString GetShortName() {return CString("button");}
        virtual CString GetVarDeclaration();
        virtual CString GetControlBlockName() {return CString("GX_BUTTON");}

        virtual CString DeclarePropertiesStruct() {return CString("");}
        virtual CString GetCreateFromDefFunctionName();
        virtual CString GetCreateFromDefFunction(int version);

        virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
        virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
        virtual void ReadFromProject(xml_reader &, studiox_project *project, int display, widget_info *info, ULONG valid_styles);

};

#endif
