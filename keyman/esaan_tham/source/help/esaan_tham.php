<?php
  $pagename = 'Lao/Esaan Tham Keyboard Help';
  $pagetitle = $pagename;
  require_once('header.php');

  $pagestyle = <<<END
    .tham { font-family: "Khottabun"; }
  END;

?>
<div>

<h1>Start Using Esaan Tham</h1>

<p>
Lao/Esaan Tham keyboard layout based on Thai Kedmanee, with automatic leading vowel reordering.
</p>

<h2>The Input Scheme</h2>

<p>
This keyboard tries to make Lao/Esaan Tham text entry in standard Unicode as natural as possible for most Thai users. It maps keys from the Kedmanee-based <a href=https://www.nectec.or.th/it-standards/std820/std820.html">TIS 820-2538</a> keyboard layout into their corresponding Tai Tham Unicode character codepoints, and assigns those with no correspondence based on their shapes or functions as much as possible.
</p>

<p>Furthermore, it also <em>reorders</em> leading vowel with the following consonant, to produce Unicode logical order from the input sequence Thai users are familar to.</p>

<p>
With this scheme, Thai users should be able to type text, Pali or Thai/Lao, as if they were using Thai keyboard, with a little vowel adjustment, like SARA E + SARA AA for Pali SARA O.
</p>

<p>
For example:
</p>

<table border="1">
<tr>
  <th>Thai script</th>
  <th>Typing order</th>
  <th>Tham script</th>
</tr>
<tr>
  <td>นโม พุทฺธาย</	td>
  <td><span class="tham">ᨶ ᩮ ᨾ ᩣ SPACE ᨻ ᩩ ᨴ ᩠ ᨵ ᩣ ᨿ</span></td>
  <td><span class="tham">ᨶᨾᩮᩣ ᨻᩩᨴ᩠ᨵᩣᨿ</span></td>
</tr>
<tr>
  <td>สมฺปนฺโน</	td>
  <td><span class="tham">ᩈ ᨾ ᩠ ᨸ ᨶ ᩠ ᩮ ᨶ ᩣ</span></td>
  <td><span class="tham">ᩈᨾ᩠ᨸᨶ᩠ᨶᩮᩣ</span></td>
</tr>
<tr>
  <td>พระพุทธเจ้า</	td>
  <td><span class="tham">ᨻ ᩕ ᨻ ᩩ ᨴ ᩠ ᨵ ᩮ ᨧ ᩫ ᩣ</span></td>
  <td><span class="tham">ᨻᩕᨻᩩᨴ᩠ᨵᨧᩮᩫᩣ</span></td>
</tr>
</table>

<h2>Key Assignments</h2>

<p>
First, it maps Thai keys in TIS 820-2538 layout into their corresponding Tham characters for those that can be directly mapped. For those without direct correspondence, it tries to match characters with similar functions or shapes. For example:
</p>

<ul>
  <li> Tham MAI KONG ( ᩫ) is assigned to Thai MAITAIKHU ( ็) key (QWERTY: H), due to their close function in syllables. </li>
  <li> Tham MEDIAL RA (ᩕ ) is assigned to open parenthesis key (QWERTY: Z), due to their similar shapes. </li>
  <li> Tham Independent SARA I (ᩍ) is assigned to Thai MAI TRI key (QWERTY: U), for their similar shapes. </li>
  <li> Tham Independent SARA II (ᩎ) is assigned to Thai MAI CHATTAWA key (QWERTY: J), after the Independent SARA I assignment. </li>
  <li> Tham Independent SARA U (ᩏ) is assigned to Thai DO CHADA key (QWERTY: E), for their similar shapes. </li>
  <li> Tham Independent SARA U (ᩐ) is assigned to question mark key (QWERTY: M), for their similar shapes. </li>
  <li> Tham Tall SARA AA ( ᩤ) is assigned to Thai LAKKHANGYAO (ๅ) key (QWERTY: !), for their similar shapes and functions. </li>
  <li> Tham SARA OY ( ᩭ) is assigned to Thai slash (/) key (QWERTY: 2), for their similar shapes. </li>
  <li> Tham YO YADNAM (ᩀ) is assigned to Thai BAHT (฿) key (QWERTY: 1), so it stays close to SARA OY. </li>
  <li> Tham MAI KANG LAI ( ᩘ) is assigned to Thai YAMAKKAN key (QWERTY: &amp;), for their similar shapes and position. </li>
  <li> Tham FINAL NGA ( ᩙ) is assigned to Shifted Thai NGO NGU key (QWERTY: "), for their similar function as syllable ending. </li>
  <li> Tham Independent SARA E (ᩑ) is assigned to close parenthesis key (QWERTY: X), as it's the key with closest shape left. </li>
</ul>

<p>
Some characters can be typed using Right Alt, such as Tham independent SARA O (ᩒ), which is assigned to the same key as the dependent SARA O (ᩰ). With this scheme, other Tham independent vowels are also assigned to the same key as the corresponding dependent ones as an alternative inputting method, apart from the normal keys described above. Therefore:
</p>

<ul>
  <li> Tham independent SARA I can be typed either by the Thai MAI TRI key (QWERTY: U) or RightAlt + dependent SARA I (QWERTY: RightAlt+b). </li>
  <li> Tham independent SARA II, either by Thai MAI CHATTAWA key (QWERTY: J) or RightAlt + dependent SARA II (QWERTY: RightAlt+u). </li>
  <li> Tham independent SARA E, either by Thai close parenthesis key (QWERTY: X) or RightAlt + dependent SARA E (QWERTY: RightAlt+g). </li>
  <li> Tham independent SARA U, either by Thai DO CHADA key (QWERTY: E) or RightAlt + dependent SARA U (QWERTY: RightAlt+6). </li>
  <li> Tham independent SARA UU, either by question mark key (QWERTY: M) or RightAlt + dependent SARA UU (QWERTY: RightAlt+Shift+6). </li>
</ul>

<p>
Some keys that are not mentioned above can be found in the full layout in the next section.
</p>


<h2>Desktop Keyboard layouts</h2>
<div id='osk' data-states='default shift rightalt rightalt-shift'>
</div>

<h2>Touch Keyboard layouts</h2>
<div id='osk-phone' data-states='default shift numeric symbol'>
</div>

</div>


