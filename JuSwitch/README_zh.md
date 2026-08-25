<font size="6">**JuSwitch**</font>

---

<div style="text-align: center;">
	<a href="./README.md">English</a>
	|
	<a href="./README_zh.md">中文</a>
</div>

## <font size="6">**目录**</font>

- [需求](#需求)
- [测试环境](#测试环境)
- [示例](#示例)
- [枚举值](#枚举值)
	- [EventListen](#eventlisten)
	- [PaddingBase](#paddingbase)
- [结构体](#结构体)
	- [ColorSet](#colorset)
		- [语法](#语法)
		- [成员](#成员)
			- [button\_color](#button_color)
			- [slider\_color](#slider_color)
		- [函数](#函数)
	- [StyleSet](#styleset)
		- [语法](#语法-1)
		- [成员](#成员-1)
	- [ParamSet](#paramset)
		- [语法](#语法-2)
		- [成员](#成员-2)
- [信号槽](#信号槽)
	- [`void SignalStateChange(bool current_state)`](#void-signalstatechangebool-current_state)
- [函数](#函数-1)
	- [JuSwitch(QWidget\* parent = nullptr, ju\_switch::ParamSet param = {}, ju\_switch::ColorSet color\_set = {}, ju\_switch::StyleSet style\_set = {})](#juswitchqwidget-parent--nullptr-ju_switchparamset-param---ju_switchcolorset-color_set---ju_switchstyleset-style_set--)
	- [~JuSwitch()](#juswitch)
	- [ju\_switch::StyleSet StyleSet()](#ju_switchstyleset-styleset)
	- [void StyleSet(ju\_switch::StyleSet new\_style\_set)](#void-stylesetju_switchstyleset-new_style_set)
	- [ju\_switch::ParamSet ParamSet()](#ju_switchparamset-paramset)
	- [void ParamSet(ju\_switch::ParamSet new\_param\_set)](#void-paramsetju_switchparamset-new_param_set)
	- [ju\_switch::ColorSet ColorSet()](#ju_switchcolorset-colorset)
	- [void ColorSet(ju\_switch::ColorSet new\_color\_set)](#void-colorsetju_switchcolorset-new_color_set)
	- [bool CurrentState()](#bool-currentstate)
	- [void ChangeState(bool new\_state, bool emit\_signal)](#void-changestatebool-new_state-bool-emit_signal)

# 需求

- **Qt**: ` >= 6.5` ( 如果Qt版本小于6.5，请删除所有与 `JuSwitch::ColorSchemeChangedSlot` 相关代码 )

# 测试环境

> **Qt**: `6.11.1`  
> **msvc**: `v143`

# 示例

<table style="width: 100%; table-layout: fixed">
	<thead>
		<tr>
			<th style="width: 20%; text-align: center">图例</th>
			<th style="width: 80%; text-align: center">参数</th>
		</tr>
	</thread>
	<tbody>
		<tr>
			<td>
				<ul style="gap: 10px; display: flex; flex-direction: column">
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold">on</span>
							<img src="./imgs/icon_default_on.png">
						</div>
					</li>
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold">off</span>
							<img src="./imgs/icon_default_off.png">
						</div>
					</li>
				</ul>
			</td>
			<td style="text-align: center;">
				<span> ( 默认参数 )</span>
			</td>
		</tr>
		<tr>
			<td>
				<ul style="gap: 10px; display: flex; flex-direction: column">
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold">on</span>
							<img src="./imgs/icon_2_on.png">
						</div>
					</li>
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold;">off</span>
							<img src="./imgs/icon_2_off.png">
						</div>
					</li>
				</ul>
			</td>
			<td>
				<pre>
					<code>
/*		完整代码见example.cpp中的"Mixed Example"部分		*/
ColorSet.button_color = 
{
	.color_light_on = QColor{"#5cbbff"},
	.color_dark_on = QColor{"#5cbbff"},
	.color_light_off = QColor{"#a6a6a6"},
	.color_dark_off = QColor{"#a6a6a6"},
	.border_light_on = QColor{"#5cbbff"},
	.border_dark_on = QColor{"#5cbbff"},
	.border_light_off = QColor{"#a6a6a6"},
	.border_dark_off = QColor{"#a6a6a6"},
}
StyleSet = 
{
	.margin = "10%",
	.padding_x_base = ju_switch::PaddingBase::BUTTON,
	.padding_h = "-50%",
	.padding_v = "20%",
}
					</code>
				</pre>
			</td>
		</tr>
			<td>
				<ul style="gap: 10px; display: flex; flex-direction: column">
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold">on</span>
							<img src="./imgs/icon_change_scheme_on.png">
						</div>
					</li>
					<li>
						<div style="display: flex; align-items: center; gap: 10px;">
							<span style="font-weight: bold">off</span>
							<img src="./imgs/icon_change_scheme_off.png">
						</div>
					</li>
				</ul>
			</td>
			<td>
				<pre>
					<code>
/*	完整代码见example.cpp中的"Switch Change Scheme Example"部分	*/
ColorSet.button_color = 
{
	.color_dark_off = QColor{ "#000" },
	.border_dark_off = QColor{ "#000" },
}
ColorSet.slider_color = 
{
	.color_dark_off = QColor{ "#c9c9c9" },
}
StyleSet = 
{
	.icon_when_true = (icon when scheme is light)
	.icon_when_false = (icon when scheme is dark)
}
					</code>
				</pre>
			</td>
		<tr>
		</tr>
	</tbody>
</table>

# 枚举值

## EventListen

**该开关的触发方式, 默认监听<kbd>鼠标左键</kbd> 和 <kbd>空格键</kbd>的抬起事件**

<table>
	<thead>
		<tr>
			<th style="text-align: center;">
			枚举值名称
			</th>
			<th style="text-align: center;">
				十进制
			</th>
			<th style="text-align: center;">
				对应按键
			</th>
		</tr>
    </thead>
	<tbody>
		<tr>
			<td style="text-align: center;">
				<code>MOUSE_LDOWN</code>
			</td>
			<td style="text-align: center;">
				1
			</td>
			<td>
				鼠标左键按下
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>MOUSE_LUP</code>
			</td>
			<td style="text-align: center;">
				2
			</td>
			<td>
				鼠标左键抬起
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>MOUSE_RDOWN</code>
			</td>
			<td style="text-align: center;">
				4
			</td>
			<td>
				鼠标右键按下
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>MOUSE_RUP</code>
			</td>
			<td style="text-align: center;">
				8
			</td>
			<td>
				鼠标右键抬起
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>KEY_ENTER_DOWN</code>
			</td>
			<td style="text-align: center;">
				16
			</td>
			<td>
				<kbd>Enter</kbd> 键按下
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>KEY_ENTER_UP</code>
			</td>
			<td style="text-align: center;">
				32
			</td>
			<td>
				<kbd>Enter</kbd> 键抬起
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>KEY_SPACE_DOWN</code>
			</td>
			<td style="text-align: center;">
				64
			</td>
			<td>
				<kbd>空格</kbd> 键按下
			</td>
		</tr>
		<tr>
			<td style="text-align: center;">
				<code>KEY_SPACE_UP</code>
			</td>
			<td style="text-align: center;">
				128
			</td>
			<td>
				<kbd>空格</kbd> 键抬起
			</td>
		</tr>
	</tbody>
</table>

## <span id="PaddingBase">PaddingBase</span>

**该枚举值指定滑块基于哪个`QWidget`计算`padding`值**

![](./imgs/PaddingBase.png)

- 当`StyleSet.padding_h` 以 `%` 结尾:
    - `WIDGET`: 基于整个`QWidget`: `StyleSet.padding_h * width`
    - `BUTTON`: 基于**按钮**的中心点: `(icon_size / 2) + StyleSet.padding_h * (icon_size / 2)`
- 当`StyleSet.padding_h` 以 `px` 结尾:
    - `WIDGET`: 基于整个`QWidget`: `StyleSet.padding_h`
    - `BUTTON`: 基于**按钮**中心点: `(icon_size / 2) + StyleSet.padding_h`

# 结构体

## ColorSet

### 语法

```cpp
	struct BaseColorSet
	{
		QColor color_light_on;
		QColor color_dark_on;
		QColor color_light_off;
		QColor color_dark_off;
		QColor border_light_on;
		QColor border_dark_on;
		QColor border_light_off;
		QColor border_dark_off;

		static BaseColorSet ButtonDefaultColor()
		{
			return
			{
				.color_light_on = QColor{ "#fff" },
				.color_dark_on = QColor{ "#fff" },
				.color_light_off = QColor{ "#fff" },
				.color_dark_off = QColor{ "#fff" },
				.border_light_on = QColor{ "#a6a6a6" },
				.border_dark_on = QColor{ "#a6a6a6" },
				.border_light_off = QColor{ "#a6a6a6" },
				.border_dark_off = QColor{ "#a6a6a6" },
			};
		}

		static BaseColorSet SliderDefaultColor()
		{
			return
			{
				.color_light_on = QColor{ "#9cd6ff" },
				.color_dark_on = QColor{ "#9cd6ff" },
				.color_light_off = QColor{ "#a6a6a6" },
				.color_dark_off = QColor{ "#a6a6a6" },
				.border_light_on = QColor{ "#9cd6ff" },
				.border_dark_on = QColor{ "#9cd6ff" },
				.border_light_off = QColor{ "#a6a6a6" },
				.border_dark_off = QColor{ "#a6a6a6" },
			};
		}
	};

	struct ColorSet
	{
		BaseColorSet button_color = BaseColorSet::ButtonDefaultColor();
		BaseColorSet slider_color = BaseColorSet::SliderDefaultColor();
	};
```

### 成员

**本处只展示系统主题为“浅色”情况, 要自定义“深色”主题, 请将变量名中的`light`改成`dark`**

#### button_color

![](./imgs/ButtonColorSet.png)

#### slider_color

![](./imgs/SliderColorSet0.png)

### 函数

- `BaseColorSet::ButtonDefaultColor`: 获取按钮的默认配色方案
- `BaseColorSet::SliderDefaultColor`: 获取滑块的默认配色方案

> [!NOTE]
> 想查阅如何使用，请看 `example.cpp` 中有关 `Change Color Example` or `Mixed Example` or `Switch Change Scheme Example` 的部分.

## StyleSet

### 语法

```cpp
	struct StyleSet
	{
		double min_width_height_rate;
		QString slider_radius;
		QString button_radius;
		QString margin;

		PaddingBase padding_x_base;
		QString padding_h;
		QString padding_v;

		QPixmap icon_when_true;
		QPixmap icon_when_false;
	};
```

### 成员

<table>
	<thead>
		<tr>
			<th style="text-align: center;">变量名</th>
			<th style="text-align: center;">变量类型</th>
			<th style="text-align: center;">默认值</th>
			<th style="text-align: center;">描述</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td style="text-align: center;"><code>min_width_height_rate</code></td>
			<td style="text-align: center;"><code>double</code></td>
			<td style="text-align: center;"><code>2.0</code></td>
			<td>宽度/高度的最小比值，请不要低于1.0，否则样式会变得很奇怪！</td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>slider_radius</code></td>
			<td style="text-align: center;"><code>QString</code></td>
			<td style="text-align: center;"><code>"50%"</code></td>
			<td>滑块的 radius, 可接受 <code>"px"</code> 和 <code>"%"</code></td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>button_radius</code></td>
			<td style="text-align: center;"><code>QString</code></td>
			<td style="text-align: center;"><code>"50%"</code></td>
			<td>按钮的 radius, 可接受 <code>"px"</code> 和 <code>"%"</code></td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>margin</code></td>
			<td style="text-align: center;"><code>QString</code></td>
			<td style="text-align: center;"><code>"0%"</code></td>
			<td>按钮和滑块到整体<code>QWidget</code>的上、下、左、右边距</td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>padding_x_base</code></td>
			<td style="text-align: center;"><code>ju_switch::PaddingBase</code></td>
			<td style="text-align: center;"><code>ju_switch::PaddingBase::WIDGET</code></td>
			<td rowspan="2">见 <a href="#PaddingBase">PaddingBase</a></td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>padding_h</code></td>
			<td style="text-align: center;"><code>QString</code></td>
			<td style="text-align: center;"><code>"0%"</code></td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>padding_v</code></td>
			<td style="text-align: center;"><code>QString</code></td>
			<td style="text-align: center;"><code>"0%"</code></td>
			<td>滑块的垂直内边距, 基于 (整体部件的高度 - margin值 * 2)</td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>icon_when_true</code></td>
			<td style="text-align: center;"><code>QPixmap</code></td>
			<td></td>
			<td>当前状态为<code>true</code>时，按钮上显示的图标</td>
		</tr>
		<tr>
			<td style="text-align: center;"><code>icon_when_false</code></td>
			<td style="text-align: center;"><code>QPixmap</code></td>
			<td></td>
			<td>当前状态为<code>false</code>时，按钮上显示的图标</td>
		</tr>
	</tbody>
</table>

## ParamSet  

### 语法  

``` cpp
struct ParamSet
{
	bool default_state;	
	int animation_duration_ms;
	
	EventListen events = EventListen::MOUSE_LUP | EventListen::KEY_SPACE_UP;
};
```

### 成员

| Name | Type | Default value | Description |
| :--: | :--: | :--: | :-- |
| `default_state` | `bool` | `false` | 开关的默认值（仅在**构造函数**中生效!） |
| `animation_duration_ms` | `int` | `150` | 动画时长（毫秒）, 动画进行期间开关不会被二次触发 |
| `events` | `ju_switch::EventListen` | `EventListen::MOUSE_LUP \| ju_switch::EventListen::KEY_SPACE_UP` | 见 [EventListen](#eventlisten) | 

# 信号槽  

## `void SignalStateChange(bool current_state)`
当 `ChangeState` 被调用，且新状态不等于当前状态时触发.

# 函数  

## JuSwitch(QWidget* parent = nullptr, [ju_switch::ParamSet](#paramset) param = {}, [ju_switch::ColorSet](#colorset) color_set = {}, [ju_switch::StyleSet](#styleset) style_set = {})

构造函数, 可以像 `auto a = new JuSWitch()` 进行无参调用(不推荐像这样不指定`parent`!!!).

## ~JuSwitch()

析构函数.

## [ju_switch::StyleSet](#styleset) StyleSet()

获取当前的 StyleSet.

## void StyleSet([ju_switch::StyleSet](#styleset) new_style_set)

设置新的 StyleSet. 会自动设置按钮和滑块的样式.

## [ju_switch::ParamSet](#paramset) ParamSet()  

获取当前 ParamSet.

## void ParamSet([ju_switch::ParamSet](#paramset) new_param_set)

设置新的 ParamSet. 会自动设置滑块的 param.

## [ju_switch::ColorSet](#colorset) ColorSet()  

获取当前的 ColorSet.

## void ColorSet([ju_switch::ColorSet](#colorset) new_color_set)

设置新的 ColorSet. 会自动设置按钮和滑块的`ColorSet`.

## bool CurrentState()

获取当前状态.

## void ChangeState(bool new_state, bool emit_signal)

设置新的状态. 如果`emit_signal == true`(默认值)会发出信号[SignalStateChange](#void-signalstatechangebool-current_state).

