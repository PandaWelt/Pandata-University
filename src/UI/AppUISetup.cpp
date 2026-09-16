#include <iostream>
#include <cstdint>
#include "AppUISetup.h"
#include "imgui.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

#define OBJECT_TYPE_LIST \
	X(BasicBox) \
	X(EditableTextBox) \
	X(EditableText) \
	X(DefinedTextBox) \
	X(DefinedText) 



enum class ObjectType

{
	#define X(name) name,
		OBJECT_TYPE_LIST
	#undef X
};

const char* ObjectTypeName(const ObjectType& type)
{
	switch (type)
	{
		#define X(name) case ObjectType::name: return #name;
			OBJECT_TYPE_LIST
		#undef X
	}
	return "unknown";
};


class BaseObjects
{
	public:
		ObjectType type = ObjectType::BasicBox;
		std::uint64_t id = 0;
		std::string name = "No Name";

		float posX = 100.0f;
		float posY = 100.0f;
		float sizeX = 100.0f;
		float sizeY = 100.0f;

		float cornerRadius = 0.5f;
		float borderThickness = 0.5f;

		ImU32 fillColor = IM_COL32(255, 0, 255, 255);
		ImU32 borderColor = IM_COL32(0, 255, 255, 255);

		float padding = 10.0f;

		char font;
		float fontSize = 20.0f;
		int characterLimit = 512;
		char textContent[512] = "";
		const char* textPlaceholder = "type here";
		ImU32 textPlaceholderColor = IM_COL32(0, 255, 0, 100);
		ImU32 textColor = IM_COL32(0, 255, 0, 255);

		void Draw(const char* id)
		{
			switch (type)
			{
				#define X(name) case ObjectType::name:	\
				name(id); break;
				OBJECT_TYPE_LIST
				#undef X
			}
		}

	private:

		void BasicBox(const char* id)
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			ImVec2 box_min(posX, posY);
			ImVec2 box_max(sizeX + posX, sizeY + posY);
			float corner_radius = cornerRadius;
			float border_thickness = borderThickness;

			ImU32 fill_color = fillColor;
			ImU32 border_color = borderColor;
			draw_list->AddRectFilled(box_min, box_max, fill_color, corner_radius);
			draw_list->AddRect(box_min, box_max, border_color, corner_radius, 0, border_thickness);
		};

		void EditableTextBox(const char* id)
		{
			BasicBox(id);

			ImVec2 input_pos(posX + padding, posY + padding);

			ImVec2 input_size(sizeX - (padding * 2.0f), sizeY - (padding * 2.0f));

			ImGui::SetCursorScreenPos(input_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 2.0f));

			ImGui::PushFont(nullptr, fontSize);
			ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_WordWrap;

			ImGui::InputTextMultiline(id, textContent, IM_ARRAYSIZE(textContent), input_size, input_flags);

			bool is_active = ImGui::IsItemActive();
			bool is_empty = textContent[0] == '\0';

			if (is_empty && !is_active)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->AddText(input_pos, textPlaceholderColor, textPlaceholder);
			};

			ImGui::PopFont();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(5);
		}

		void EditableText(const char* id)
		{
			ImVec2 input_pos(posX, posY);

			ImVec2 input_size(sizeX, sizeY);

			ImGui::SetCursorScreenPos(input_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 2.0f));

			ImGui::PushFont(nullptr, fontSize);
			ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_WordWrap;

			ImGui::InputTextMultiline(id, textContent, IM_ARRAYSIZE(textContent), input_size, input_flags);

			bool is_active = ImGui::IsItemActive();
			bool is_empty = textContent[0] == '\0';

			if (is_empty && !is_active)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->AddText(input_pos, textPlaceholderColor, textPlaceholder);
			};

			ImGui::PopFont();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(5);
		}

		void DefinedTextBox(const char* id)
		{
			BasicBox(id);

			ImVec2 input_pos(posX + padding, posY + padding);

			ImVec2 input_size(sizeX - (padding * 2.0f), sizeY - (padding * 2.0f));

			ImGui::SetCursorScreenPos(input_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 2.0f));

			ImGui::PushFont(nullptr, fontSize);
			ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_WordWrap;

			ImGui::InputTextMultiline(id, textContent, IM_ARRAYSIZE(textContent), input_size, input_flags);

			bool is_active = ImGui::IsItemActive();
			bool is_empty = textContent[0] == '\0';

			if (is_empty && !is_active)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->AddText(input_pos, textPlaceholderColor, textPlaceholder);
			};

			ImGui::PopFont();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(5);
		}

		void DefinedText(const char* id)
		{
			ImVec2 input_pos(posX, posY);

			ImVec2 input_size(sizeX, sizeY);

			ImGui::SetCursorScreenPos(input_pos);
			ImGui::PushStyleColor(ImGuiCol_Text, textColor);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, IM_COL32(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 0));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 2.0f));

			ImGui::PushFont(nullptr, fontSize);
			ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_WordWrap;

			ImGui::InputTextMultiline(id, textContent, IM_ARRAYSIZE(textContent), input_size, input_flags);

			bool is_active = ImGui::IsItemActive();
			bool is_empty = textContent[0] == '\0';

			if (is_empty && !is_active)
			{
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				draw_list->AddText(input_pos, textPlaceholderColor, textPlaceholder);
			};

			ImGui::PopFont();
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(5);
		}
};

enum class InterpolationType
{
	linear,
	easeIn,
	easeOut,
	easeInOut,
	bounceIn,
	bounceOut,
};
class Animation
{
public:
	Animation(
		float start,
		float end,
		float seconds,
		InterpolationType type
	)
	{
		startValue = start;
		finalValue = end;
		animLength = seconds;
		interpType = type;
	}

	float animLength = 1.0f;


	void trigger(){
		captureStartTime();}

	float getValue(){
		return interpValue();}
	float readProgres(){
		return getProgress();}

private:
	float currentValue;
	float startValue;
	float finalValue;
	double startTime;
	bool isRunning = false;

	InterpolationType interpType = 
		InterpolationType::linear;


	void captureStartTime()
	{
		startTime = ImGui::GetTime();
		isRunning = true;
	}

	double getStartTime() const {
		return startTime;}

	float getProgress() const
	{
		float elapsed = ImGui::GetTime() - getStartTime();
		return std::clamp(elapsed / animLength, 0.0f,1.0f);
	}

	float interpValue()
	{
		float progress = getProgress();
		float easedProgress = interpolate(progress);
		float delta = finalValue - startValue;
		if (!isRunning){
			return startValue;}
		if (isRunning)
		{
			currentValue = startValue + delta * easedProgress;

			return currentValue;
		}
	}
	float interpolate(float progress) const
	{
		switch (interpType)
		{
		case InterpolationType::linear:
			return progress;

		case InterpolationType::easeIn:
			return progress * progress;
		
		case InterpolationType::easeOut:
			return 1.0f - (1.0f - progress) * (1.0f - progress);

		case InterpolationType::easeInOut:
			return progress < 0.5f
				? 2.0f * progress * progress
				: 1.0f -
				std::pow( -2.0f * progress + 2.0f, 2.0f) / 2.0f;
		case InterpolationType::bounceOut:
		{
			if (progress < 1.0f / 2.75f)
			{
				return 7.5625f * progress * progress;
			}
			else if (progress < 2.0f / 2.75f)
			{
				progress -= 1.5f / 2.75f;
				return 7.5625f * progress * progress + 0.75f;
			}
			else if (progress < 2.5f / 2.75f)
			{
				progress -= 2.25f / 2.75f;
				return 7.5625f * progress * progress + 0.9375f;
			}
			else
			{
				progress -= 2.625f / 2.75f;
				return 7.5625f * progress * progress + 0.984375f;
			}
		}
		}
	};
};

class ObjectSpawner
{
public:

BaseObjects object;

	void spawn(const ObjectType& type)
	{
		object.id = nextId++;
		object.name = std::string(ObjectTypeName(type)) + std::string("_") + std::to_string(object.id);
		object.type = type;
		sceneObjects.emplace_back(object);
		PrintVector;
	}
	void drawAll()
	{
		for (BaseObjects& obj : sceneObjects)
		{
			ImGui::PushID(static_cast<int>(object.id));

			object.Draw("##object");
			ImGui::PopID();
		}
	}

	static void PrintVector(const std::vector<BaseObjects>& objects)
	{
		std::cout<< "size: " << objects.size() << std::endl;
		if (objects.empty())
			return;
		std::cout << "Elements: { ";
		for (int i = 0; i < objects.size(); i++)
		{
			std::cout << "vector " << objects[i].id;
		}
	}

private:
	std::vector<BaseObjects>sceneObjects;
	std::uint64_t nextId = 1;
};

class PopupEditor
{
public:
	bool spawnRequested = false;

	ObjectType requestedObject;

	void DrawPopupWindow() {
		PopupWindow(); }

	void SpawnRequestButton() {
		SpawnButton(); }

private:

	void SpawnButton()
	{
		if (ImGui::Button("Spawn Object"))
			ImGui::OpenPopup("Object Selection");
			ImGui::SameLine();

			if (ImGui::BeginPopup("Object Selection"))
			{
				#define X(name)								\
					if (ImGui::Button(#name)) {				\
					requestedObject = ObjectType::name;		\
					spawnRequested = true;}
				OBJECT_TYPE_LIST
				#undef X(name)

				ImGui::EndPopup();
			}
	};

	void PopupWindow()
	{
		ImGuiWindowFlags popup_flags =
			ImGuiWindowFlags_None;
		ImGui::Begin("PopupWindow", nullptr);

		SpawnRequestButton();

		ImGui::End();
	}
};


void AppUISetup(ImVec4& clear_color)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(io.DisplaySize);

	ImGuiWindowFlags canvas_flags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::Begin("Canvas", nullptr, canvas_flags);

	static PopupEditor popup; {
		popup.DrawPopupWindow(); }


	static ObjectSpawner spawner;
	{
			if (popup.spawnRequested) {
				spawner.spawn(popup.requestedObject); }

			spawner.drawAll();
	}

	static BaseObjects box2;
	{
		static Animation R
			(50, 200, 5.0f, InterpolationType::bounceOut);
		static Animation G
		(50, 200, 5.0f, InterpolationType::bounceOut);
		box2.sizeX = 50.0f;
		box2.posX = 200.0f;
		box2.posY = 200.0f;
		box2.fillColor = IM_COL32(R.getValue(), G.getValue(), 200, 255);

		if (ImGui::IsKeyPressed(ImGuiKey_Space))
		{
			R.trigger();
			G.trigger();
		}
		box2.type = ObjectType::EditableTextBox;
		box2.Draw("##box2");
	}

	   

	ImGui::End();

}