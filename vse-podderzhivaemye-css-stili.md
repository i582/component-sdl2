# Все поддерживаемые CSS стили

| Attribute | Value type | Description |
| :--- | :--- | :--- |
| **Size** |  |  |
| `width` | The width of the component | `[number + px/number + %/calc((expr -> X% +(-) Ypx))]` |
| `height` | The height of the component | `same` |
| `top` | Shift the component from the top edge \(Y\) | `same` |
| `left` | Shift the component from the left edge \(X\) | `same` |
| **Background** |  |  |
| `background-color` | The background color of the component. | `[hex/rgb/rgba]` |
| `background-image` | The background image of the component. | `[url(path_to_image_without_quotes)]` |
| `background-position-x` | Shift the background image by X | `[number + px]` |
| `background-position-y` | Shift the background image by Y | `[number + px]` |
| `background-size` | Background image width \(height proportional\) | `[number + px]` |
| **Text** |  |  |
| `color` | Color of the text in the component. | `[hex/rgb/rgba]` |
| `font-size` | Font size in the component | `[number + px]` |
| `font-family` | Font family in the component | `[string]` |
| `font-style` | Font style in the component | `[normal/italic]` |
| `font-weight` | Weight of the font in the component | `[100-900]` |
| `line-height` | Line height of the font in the component | `[double]` |
| `text-align` | Alignment of the text horizontally in the component | `[left/center/right]` |
| `vertical-align` | Alignment of the text vertically in the component. | `[top/center/bottom]` |
| `margin-[top/bottom/left/right]` | Shift the text in the directions in the component. | `[number + px]` |
| **Border** |  |  |
| `border-color` | Color of the border of the component. | `[hex/rgb/rgba]` |
| `border-radius` | The radius of the curve of the stroke in the component | `[number + px]` |
| `border-[top/bottom/left/right]` | Border | `[number + px hex solid]` |
| `border-[top/bottom/left/right]-size` | Width of the border in the component | `[number + px]` |
| `border-[top/bottom/left/right]-color` | Color of the border in the component | `[hex]` |
| `border-[top/bottom/left/right]-type` | Type of the border in the component | `[solid]` |
| `outline` | Single pixel border. | `[hex/rgb/rgba]` |
| **Display** |  |  |
| `display` | Display of child elements. `block` - alignment of elements in a column. `inline-block` - alignment of elements in a row. | `[block/inline-block/unset]` |
| **Scroll** |  |  |
| `overflow` | Determines whether the scroll is added if the child components are outside the parent. | `[hidden/unset]` |

