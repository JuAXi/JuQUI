<font size="6">**JuButton**</font>

---

<div style="text-align: center;">
	<a href="./README.md">English</a>
	|
	<span>中文</span>
</div>

## <font size="6">**ToC**</font>

- [需求](#需求)
- [测试环境](#测试环境)
- [Example](#example)
- [结构体](#结构体)
	- [ColorSet](#colorset)
		- [语法](#语法)
		- [图片及对应参数](#图片及对应参数)
- [信号槽](#信号槽)
	- [`void SignalButtonPress()`](#void-signalbuttonpress)
	- [`void SignalButtonRelease()`](#void-signalbuttonrelease)
- [函数](#函数)
	- [void SetBaseColor(ju\_button::ColorSet colors)](#void-setbasecolorju_buttoncolorset-colors)
	- [void SetHoverColor(ju\_button::ColorSet colors)](#void-sethovercolorju_buttoncolorset-colors)
	- [void SetPressColor(ju\_button::ColorSet colors)](#void-setpresscolorju_buttoncolorset-colors)
	- [void SetAnimationDuration(int ms)](#void-setanimationdurationint-ms)
	- [void SetText(QString text)](#void-settextqstring-text)
	- [void SetFont(QFont font)](#void-setfontqfont-font)
	- [void SetPadding(int padding)](#void-setpaddingint-padding)
	- [void SetPadding(int top, int right, int bottom, int left)](#void-setpaddingint-top-int-right-int-bottom-int-left)

# 需求

- **Qt**: ` >= 6`

# 测试环境

> **Qt**: `6.11.1`  
> **msvc**: `v143`

# Example

<table style="width: 100%; table-layout: fixed">
	<thead>
		<tr>
			<th style="width: 20%; text-align: center">图片</th>
			<th style="width: 80%; text-align: center">参数</th>
		</tr>
	</thread>
	<tbody>
		<tr>
			<td>
				<ul style="gap: 10px; display: flex; flex-direction: column">
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold; white-space: nowrap">默认状态</span>
							<img style="width: 100px, height: 50px" src="./img/button_default.png">
						</div>
					</li>
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold; white-space: nowrap">按钮悬停</span>
							<img style="width: 100px, height: 50px" src="./img/button_hover.png">
						</div>
					</li>
					</li>
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold; white-space: nowrap">按钮按下</span>
							<img style="width: 100px, height: 50px" src="./img/button_pressed.png">
						</div>
					</li>
				</ul>
			</td>
			<td style="text-align: center;">
				<span> ( 默认样式 )</span>
			</td>
		</tr>
	</tbody>
</table>

# 结构体

## ColorSet

### 语法

```cpp
	struct ColorSet
	{
		QColor button_background_color;
		QColor button_border_color;
		QColor text_color;
	};
```

### 图片及对应参数 

<div style="text-align: center;">
	<img src="./img/button_color_example.png">
</div>

# 信号槽  

## `void SignalButtonPress()`  

按钮按下时触发。

## `void SignalButtonRelease()`

按钮释放时触发。

# 函数  

## void SetBaseColor([ju_button::ColorSet](#colorset) colors)  

设置按钮未按下且未悬停时的颜色。

## void SetHoverColor([ju_button::ColorSet](#colorset) colors)  

设置按钮悬停且未按下时的颜色。

## void SetPressColor([ju_button::ColorSet](#colorset) colors)  

设置按钮按下时的颜色。

## void SetAnimationDuration(int ms)  

设置动画时长。

## void SetText(QString text)  

设置按钮文本。

## void SetFont(QFont font)  

设置按钮字体。

## void SetPadding(int padding)

设置文字到边框的距离。

## void SetPadding(int top, int right, int bottom, int left)

在四个方向上分别设置文字到边框的距离。