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

#ifndef _ACCORDION_MENU_SERVICE_PROVIDER_
#define _ACCORDION_MENU_SERVICE_PROVIDER_

class screen_generator; // forward reference

class accordion_menu_service_provider : public widget_service_provider
{
public:
    accordion_menu_service_provider();

    virtual int GetType() { return GX_TYPE_ACCORDION_MENU; }
    virtual CString GetTypeName(widget_info *) { return CString("GX_TYPE_ACCORDION_MENU"); }
    virtual CString GetShortName() { return CString("accordion_menu"); }
    virtual CString GetVarDeclaration(){ return CString(""); };
    virtual CString GetControlBlockName() { return CString("GX_ACCORDION_MENU"); }

    virtual CString DeclarePropertiesStruct(){ return CString(""); };
    virtual CString WriteExtendedProperties(screen_generator *gen, CString &scope, widget_info *info){ return CString(""); };
    virtual CString GetCreateFromDefFunctionName();
    virtual CString GetCreateFromDefFunction(int version);

    virtual widget_info *CreateNewInstance(GX_WIDGET *parent);
    virtual GX_WIDGET *GenerateFromInfo(GX_WIDGET *parent, widget_info *info);
};

#endif
