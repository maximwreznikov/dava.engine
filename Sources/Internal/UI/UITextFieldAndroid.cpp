/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/


#if defined(__DAVAENGINE_ANDROID__)
#include "UITextFieldAndroid.h"
#include "Utils/UTF8Utils.h"

using namespace DAVA;

jclass JniTextField::gJavaClass = NULL;
const char* JniTextField::gJavaClassName = NULL;

jclass JniTextField::GetJavaClass() const
{
	return gJavaClass;
}

const char* JniTextField::GetJavaClassName() const
{
	return gJavaClassName;
}

JniTextField::JniTextField(uint32_t id)
{
	this->id = id;
}

void JniTextField::Create(Rect controlRect)
{
	Rect rect = V2P(controlRect);
	jmethodID mid = GetMethodID("Create", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				rect.x,
				rect.y,
				rect.dx,
				rect.dy);
	}
}

void JniTextField::Destroy()
{
	jmethodID mid = GetMethodID("Destroy", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

void JniTextField::UpdateRect(const Rect & controlRect)
{
	Rect rect = V2P(controlRect);
	jmethodID mid = GetMethodID("UpdateRect", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				rect.x,
				rect.y,
				rect.dx,
				rect.dy);
	}
}

void JniTextField::SetText(const char* text)
{
	jmethodID mid = GetMethodID("SetText", "(ILjava/lang/String;)V");
	if (mid)
	{
		jstring jStrDefaultText = GetEnvironment()->NewStringUTF(text);
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				jStrDefaultText);
		GetEnvironment()->DeleteLocalRef(jStrDefaultText);
	}
}

void JniTextField::SetTextColor(float r, float g, float b, float a)
{
	jmethodID mid = GetMethodID("SetTextColor", "(IFFFF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				r,
				g,
				b,
				a);
	}
}

void JniTextField::SetFontSize(float size)
{
	float32 v2p = Core::Instance()->GetVirtualToPhysicalFactor();
	jmethodID mid = GetMethodID("SetFontSize", "(IF)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				size * v2p);
	}
}

void JniTextField::SetIsPassword(bool isPassword)
{
	jmethodID mid = GetMethodID("SetIsPassword", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				isPassword);
	}
}

void JniTextField::SetTextAlign(int32_t align)
{
	jmethodID mid = GetMethodID("SetTextAlign", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				align);
	}
}

void JniTextField::SetTextUseRtlAlign(bool useRtlAlign)
{
	jmethodID mid = GetMethodID("SetTextUseRtlAlign", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				useRtlAlign);
	}
}

void JniTextField::SetInputEnabled(bool value)
{
	jmethodID mid = GetMethodID("SetInputEnabled", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetAutoCapitalizationType(int32_t value)
{
	jmethodID mid = GetMethodID("SetAutoCapitalizationType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetAutoCorrectionType(int32_t value)
{
	jmethodID mid = GetMethodID("SetAutoCorrectionType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetSpellCheckingType(int32_t value)
{
	jmethodID mid = GetMethodID("SetSpellCheckingType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetKeyboardAppearanceType(int32_t value)
{
	jmethodID mid = GetMethodID("SetKeyboardAppearanceType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetKeyboardType(int32_t value)
{
	jmethodID mid = GetMethodID("SetKeyboardType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetReturnKeyType(int32_t value)
{
	jmethodID mid = GetMethodID("SetReturnKeyType", "(II)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetEnableReturnKeyAutomatically(bool value)
{
	jmethodID mid = GetMethodID("SetEnableReturnKeyAutomatically", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				value);
	}
}

void JniTextField::SetVisible(bool isVisible)
{
	jmethodID mid = GetMethodID("SetVisible", "(IZ)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id,
				isVisible);
	}
}

void JniTextField::OpenKeyboard()
{
	jmethodID mid = GetMethodID("OpenKeyboard", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

void JniTextField::CloseKeyboard()
{
	jmethodID mid = GetMethodID("CloseKeyboard", "(I)V");
	if (mid)
	{
		GetEnvironment()->CallStaticVoidMethod(
				GetJavaClass(),
				mid,
				id);
	}
}

uint32 JniTextField::GetCursorPos()
{
	jmethodID mid = GetMethodID("GetCursorPos", "(I)I");
	if (!mid)
		return 0;

	return GetEnvironment()->CallStaticIntMethod(GetJavaClass(), mid, id);
}

void JniTextField::SetCursorPos(uint32 pos)
{
	jmethodID mid = GetMethodID("SetCursorPos", "(II)V");
	if (!mid)
		return;
	GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, pos);
}

void JniTextField::SetMaxLength(int32_t value)
{
	jmethodID mid = GetMethodID("SetMaxLength", "(II)V");
	if (!mid)
		return;
	GetEnvironment()->CallStaticVoidMethod(GetJavaClass(), mid, id, value);
}

uint32_t UITextFieldAndroid::sId = 0;
DAVA::Map<uint32_t, UITextFieldAndroid*> UITextFieldAndroid::controls;

UITextFieldAndroid::UITextFieldAndroid(UITextField* textField)
{
	this->textField = textField;
	id = sId++;
	rect = textField->GetRect();
	JniTextField jniTextField(id);
	jniTextField.Create(rect);

	controls[id] = this;
}

UITextFieldAndroid::~UITextFieldAndroid()
{
	controls.erase(id);

	JniTextField jniTextField(id);
	jniTextField.Destroy();
}

void UITextFieldAndroid::OpenKeyboard()
{
	JniTextField jniTextField(id);
	jniTextField.OpenKeyboard();
}

void UITextFieldAndroid::CloseKeyboard()
{
	JniTextField jniTextField(id);
	jniTextField.CloseKeyboard();
}

void UITextFieldAndroid::GetText(WideString & string) const
{
	string = text;
}

void UITextFieldAndroid::SetText(const WideString & string)
{
	if (text.compare(string) != 0)
	{
		text = TruncateText(string, textField->GetMaxLength());

		JniTextField jniTextField(id);
		String utfText = UTF8Utils::EncodeToUTF8(text);
		jniTextField.SetText(utfText.c_str());
	}
}

void UITextFieldAndroid::UpdateRect(const Rect & rect)
{
	if (rect != this->rect)
	{
		this->rect = rect;
		JniTextField jniTextField(id);
		jniTextField.UpdateRect(rect);
	}
}

void UITextFieldAndroid::SetTextColor(const DAVA::Color &color)
{
	JniTextField jniTextField(id);
	jniTextField.SetTextColor(color.r, color.g, color.b, color.a);
}

void UITextFieldAndroid::SetFontSize(float size)
{
	JniTextField jniTextField(id);
	jniTextField.SetFontSize(size);
}

void UITextFieldAndroid::SetTextAlign(DAVA::int32 align)
{
	this->align = align;
	JniTextField jniTextField(id);
	jniTextField.SetTextAlign(align);
}

DAVA::int32 UITextFieldAndroid::GetTextAlign()
{
	return align;
}

void UITextFieldAndroid::SetTextUseRtlAlign(bool useRtlAlign)
{
	this->useRtlAlign = useRtlAlign;
	JniTextField jniTextField(id);
	jniTextField.SetTextUseRtlAlign(useRtlAlign);
}

bool UITextFieldAndroid::GetTextUseRtlAlign() const
{
	return useRtlAlign;
}

void UITextFieldAndroid::SetVisible(bool isVisible)
{
	JniTextField jniTextField(id);
	jniTextField.SetVisible(isVisible);
}

void UITextFieldAndroid::SetIsPassword(bool isPassword)
{
	JniTextField jniTextField(id);
	jniTextField.SetIsPassword(isPassword);
}

void UITextFieldAndroid::SetInputEnabled(bool value)
{
	JniTextField jniTextField(id);
	jniTextField.SetInputEnabled(value);
}

// Keyboard traits.
void UITextFieldAndroid::SetAutoCapitalizationType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetAutoCapitalizationType(value);
}

void UITextFieldAndroid::SetAutoCorrectionType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetAutoCorrectionType(value);
}

void UITextFieldAndroid::SetSpellCheckingType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetSpellCheckingType(value);
}

void UITextFieldAndroid::SetKeyboardAppearanceType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetKeyboardAppearanceType(value);
}

void UITextFieldAndroid::SetKeyboardType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetKeyboardType(value);
}

void UITextFieldAndroid::SetReturnKeyType(DAVA::int32 value)
{
	JniTextField jniTextField(id);
	jniTextField.SetReturnKeyType(value);
}

void UITextFieldAndroid::SetEnableReturnKeyAutomatically(bool value)
{
	JniTextField jniTextField(id);
	jniTextField.SetEnableReturnKeyAutomatically(value);
}

uint32 UITextFieldAndroid::GetCursorPos()
{
	JniTextField jniTextField(id);
	return jniTextField.GetCursorPos();
}

void UITextFieldAndroid::SetCursorPos(uint32 pos)
{
	JniTextField jniTextField(id);
	return jniTextField.SetCursorPos(pos);
}

void UITextFieldAndroid::SetMaxLength(DAVA::int32 value)
{
	JniTextField jniTextField(id);

	WideString truncated = TruncateText(text, value);
	if (truncated != text)
	{
		SetText(truncated);
	}

	return jniTextField.SetMaxLength(value);
}

WideString UITextFieldAndroid::TruncateText(const WideString& text, int32 maxLength)
{
	WideString str = text;

	if (maxLength >= 0 && maxLength < str.length())
	{
		str.resize(maxLength);
	}

	return str;
}

bool UITextFieldAndroid::TextFieldKeyPressed(int32 replacementLocation, int32 replacementLength, WideString &text)
{
	bool res = true;
	UITextFieldDelegate* delegate = textField->GetDelegate();
	if (delegate)
		res = delegate->TextFieldKeyPressed(textField, replacementLocation, replacementLength, text);

	if (res)
	{
		WideString curText = textField->GetText();
		if (curText.length() >= replacementLocation)
		{
			curText.replace(replacementLocation, replacementLength, text);
			this->text = curText;
		}
	}
	return res;
}

bool UITextFieldAndroid::TextFieldKeyPressed(uint32_t id, int32 replacementLocation, int32 replacementLength, WideString &text)
{
	UITextFieldAndroid* control = GetUITextFieldAndroid(id);
	if (!control)
		return false;

	return control->TextFieldKeyPressed(replacementLocation, replacementLength, text);
}

void UITextFieldAndroid::TextFieldShouldReturn()
{
	UITextFieldDelegate* delegate = textField->GetDelegate();
	if (delegate)
		delegate->TextFieldShouldReturn(textField);
}

void UITextFieldAndroid::TextFieldShouldReturn(uint32_t id)
{
	UITextFieldAndroid* control = GetUITextFieldAndroid(id);
	if (!control)
		return;

	control->TextFieldShouldReturn();
}

UITextFieldAndroid* UITextFieldAndroid::GetUITextFieldAndroid(uint32_t id)
{
	DAVA::Map<uint32_t, UITextFieldAndroid*>::iterator iter = controls.find(id);
	if (iter != controls.end())
		return iter->second;

	return NULL;
}

void UITextFieldAndroid::TextFieldKeyboardShown(const Rect& rect)
{
    UITextFieldDelegate* delegate = textField->GetDelegate();
    if (delegate)
        delegate->OnKeyboardShown(rect);
}

void UITextFieldAndroid::TextFieldKeyboardShown(uint32_t id, const Rect& rect)
{
    UITextFieldAndroid* control = GetUITextFieldAndroid(id);
    if (!control)
        return;
    control->TextFieldKeyboardShown(rect);
}

void UITextFieldAndroid::TextFieldKeyboardHidden()
{
    UITextFieldDelegate* delegate = textField->GetDelegate();
    if (delegate)
        delegate->OnKeyboardHidden();
}

void UITextFieldAndroid::TextFieldKeyboardHidden(uint32_t id)
{
    UITextFieldAndroid* control = GetUITextFieldAndroid(id);
    if (!control)
        return;
    control->TextFieldKeyboardHidden();
}

void UITextFieldAndroid::TextFieldFocusChanged(bool hasFocus)
{
    if(textField)
    {
        if(hasFocus)
        {
            if (DAVA::UIControlSystem::Instance()->GetFocusedControl() != textField)
            {
                DAVA::UIControlSystem::Instance()->SetFocusedControl(textField, false);
            }
        }
        else
        {
            if (DAVA::UIControlSystem::Instance()->GetFocusedControl() == textField)
            {
                DAVA::UIControlSystem::Instance()->SetFocusedControl(NULL, false);
            }
        }
    }
}

void UITextFieldAndroid::TextFieldFocusChanged(uint32_t id, bool hasFocus)
{
    UITextFieldAndroid* control = GetUITextFieldAndroid(id);
    if (!control)
        return;
    control->TextFieldFocusChanged(hasFocus);
}
#endif
