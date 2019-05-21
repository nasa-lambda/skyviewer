#ifndef DEFINE_COLOR_TABLE_H
#define DEFINE_COLOR_TABLE_H
/* ============================================================================
The currently defined color tables.  There are two:
- the 'standard' rainbow color table
- a black and white color table.

ONLY REFERENCE THIS FILE IN colortable.cpp!

Written by Nicholas Phillips.
QT4 adaption and Black/White color table by Michael R. Greason, ADNET,
	23 August 2007.
============================================================================ */
int colortable_count = 2;
/* ----------------------------------------------------------------------------
			The default colortable
*/
char  default_ct_name[] = {"Default"};
float default_colortable[][3] = {
    {        0,        0, 0.545098},
    {        0,        0, 0.560784},
    {        0,        0, 0.576471},
    {        0,        0, 0.592157},
    {        0,        0, 0.607843},
    {        0,        0, 0.623529},
    {        0,        0, 0.639216},
    {        0,        0, 0.654902},
    {        0,        0, 0.670588},
    {        0,        0, 0.686275},
    {        0,        0, 0.701961},
    {        0,        0, 0.717647},
    {        0,        0, 0.733333},
    {        0,        0,  0.74902},
    {        0,        0, 0.764706},
    {        0,        0, 0.780392},
    {        0,        0, 0.796078},
    {        0,        0, 0.811765},
    {        0,        0, 0.827451},
    {        0,        0, 0.843137},
    {        0,        0, 0.858824},
    {        0,        0,  0.87451},
    {        0,        0, 0.890196},
    {        0,        0, 0.905882},
    {        0,        0, 0.921569},
    {        0,        0, 0.937255},
    {        0,        0, 0.952941},
    {        0,        0, 0.968627},
    {        0,        0, 0.984314},
    {        0,        0,        1},
    {        0,        0,        1},
    {        0,0.0117647,        1},
    {        0, 0.027451,        1},
    {        0,0.0431373,        1},
    {        0,0.0588235,        1},
    {        0,0.0745098,        1},
    {        0,0.0901961,        1},
    {        0, 0.105882,        1},
    {        0, 0.121569,        1},
    {        0, 0.137255,        1},
    {        0, 0.152941,        1},
    {        0, 0.168627,        1},
    {        0, 0.184314,        1},
    {        0,      0.2,        1},
    {        0, 0.215686,        1},
    {        0, 0.231373,        1},
    {        0, 0.247059,        1},
    {        0, 0.262745,        1},
    {        0, 0.278431,        1},
    {        0, 0.294118,        1},
    {        0, 0.309804,        1},
    {        0,  0.32549,        1},
    {        0, 0.341176,        1},
    {        0, 0.356863,        1},
    {        0, 0.372549,        1},
    {        0, 0.388235,        1},
    {        0, 0.403922,        1},
    {        0, 0.419608,        1},
    {        0, 0.435294,        1},
    {        0,  0.45098,        1},
    {        0, 0.466667,        1},
    {        0, 0.482353,        1},
    {        0, 0.498039,        1},
    {        0, 0.513726,        1},
    {        0, 0.529412,        1},
    {        0, 0.545098,        1},
    {        0, 0.560784,        1},
    {        0, 0.576471,        1},
    {        0, 0.592157,        1},
    {        0, 0.607843,        1},
    {        0, 0.623529,        1},
    {        0, 0.639216,        1},
    {        0, 0.654902,        1},
    {        0, 0.670588,        1},
    {        0, 0.686275,        1},
    {        0, 0.701961,        1},
    {        0, 0.717647,        1},
    {        0, 0.733333,        1},
    {        0,  0.74902,        1},
    {        0, 0.764706,        1},
    {        0, 0.780392,        1},
    {        0, 0.796078,        1},
    {        0, 0.811765,        1},
    {        0, 0.827451,        1},
    {        0, 0.843137,        1},
    {        0, 0.858824,        1},
    {        0,  0.87451,        1},
    {        0, 0.890196,        1},
    {        0, 0.905882,        1},
    {        0, 0.921569,        1},
    {        0, 0.937255,        1},
    {        0, 0.952941,        1},
    {        0, 0.968627,        1},
    {        0, 0.984314,        1},
    {        0,        1,        1},
    {        0,        1,        1},
    {0.0117647,        1, 0.984314},
    { 0.027451,        1, 0.968627},
    {0.0431373,        1, 0.952941},
    {0.0588235,        1, 0.937255},
    {0.0745098,        1, 0.921569},
    {0.0901961,        1, 0.905882},
    { 0.105882,        1, 0.890196},
    { 0.121569,        1,  0.87451},
    { 0.137255,        1, 0.858824},
    { 0.152941,        1, 0.843137},
    { 0.168627,        1, 0.827451},
    { 0.184314,        1, 0.811765},
    {      0.2,        1, 0.796078},
    { 0.215686,        1, 0.780392},
    { 0.231373,        1, 0.764706},
    { 0.247059,        1,  0.74902},
    { 0.262745,        1, 0.733333},
    { 0.278431,        1, 0.717647},
    { 0.294118,        1, 0.701961},
    { 0.309804,        1, 0.686275},
    {  0.32549,        1, 0.670588},
    { 0.341176,        1, 0.654902},
    { 0.356863,        1, 0.639216},
    { 0.372549,        1, 0.623529},
    { 0.388235,        1, 0.607843},
    { 0.403922,        1, 0.592157},
    { 0.419608,        1, 0.576471},
    { 0.435294,        1, 0.560784},
    {  0.45098,        1, 0.545098},
    { 0.466667,        1, 0.529412},
    { 0.482353,        1, 0.513726},
    { 0.498039,        1, 0.498039},
    { 0.513726,        1, 0.482353},
    { 0.529412,        1, 0.466667},
    { 0.545098,        1,  0.45098},
    { 0.560784,        1, 0.435294},
    { 0.576471,        1, 0.419608},
    { 0.592157,        1, 0.403922},
    { 0.607843,        1, 0.388235},
    { 0.623529,        1, 0.372549},
    { 0.639216,        1, 0.356863},
    { 0.654902,        1, 0.341176},
    { 0.670588,        1,  0.32549},
    { 0.686275,        1, 0.309804},
    { 0.701961,        1, 0.294118},
    { 0.717647,        1, 0.278431},
    { 0.733333,        1, 0.262745},
    {  0.74902,        1, 0.247059},
    { 0.764706,        1, 0.231373},
    { 0.780392,        1, 0.215686},
    { 0.796078,        1,      0.2},
    { 0.811765,        1, 0.184314},
    { 0.827451,        1, 0.168627},
    { 0.843137,        1, 0.152941},
    { 0.858824,        1, 0.137255},
    {  0.87451,        1, 0.121569},
    { 0.890196,        1, 0.105882},
    { 0.905882,        1,0.0901961},
    { 0.921569,        1,0.0745098},
    { 0.937255,        1,0.0588235},
    { 0.952941,        1,0.0431373},
    { 0.968627,        1, 0.027451},
    { 0.984314,        1,0.0117647},
    {        1,        1,        0},
    {        1, 0.984314,        0},
    {        1, 0.968627,        0},
    {        1, 0.952941,        0},
    {        1, 0.937255,        0},
    {        1, 0.921569,        0},
    {        1, 0.905882,        0},
    {        1, 0.890196,        0},
    {        1,  0.87451,        0},
    {        1, 0.858824,        0},
    {        1, 0.843137,        0},
    {        1, 0.827451,        0},
    {        1, 0.811765,        0},
    {        1, 0.796078,        0},
    {        1, 0.780392,        0},
    {        1, 0.764706,        0},
    {        1,  0.74902,        0},
    {        1, 0.733333,        0},
    {        1, 0.717647,        0},
    {        1, 0.701961,        0},
    {        1, 0.686275,        0},
    {        1, 0.670588,        0},
    {        1, 0.654902,        0},
    {        1, 0.639216,        0},
    {        1, 0.623529,        0},
    {        1, 0.607843,        0},
    {        1, 0.592157,        0},
    {        1, 0.576471,        0},
    {        1, 0.560784,        0},
    {        1, 0.545098,        0},
    {        1, 0.529412,        0},
    {        1, 0.513726,        0},
    {        1, 0.498039,        0},
    {        1, 0.482353,        0},
    {        1, 0.466667,        0},
    {        1,  0.45098,        0},
    {        1, 0.435294,        0},
    {        1, 0.419608,        0},
    {        1, 0.403922,        0},
    {        1, 0.388235,        0},
    {        1, 0.372549,        0},
    {        1, 0.356863,        0},
    {        1, 0.341176,        0},
    {        1,  0.32549,        0},
    {        1, 0.309804,        0},
    {        1, 0.294118,        0},
    {        1, 0.278431,        0},
    {        1, 0.262745,        0},
    {        1, 0.247059,        0},
    {        1, 0.231373,        0},
    {        1, 0.215686,        0},
    {        1,      0.2,        0},
    {        1, 0.184314,        0},
    {        1, 0.168627,        0},
    {        1, 0.152941,        0},
    {        1, 0.137255,        0},
    {        1, 0.121569,        0},
    {        1, 0.105882,        0},
    {        1,0.0901961,        0},
    {        1,0.0745098,        0},
    {        1,0.0588235,        0},
    {        1,0.0431373,        0},
    {        1, 0.027451,        0},
    {        1,0.0117647,        0},
    {        1,        0,        0},
    { 0.980392,        0,        0},
    { 0.964706,        0,        0},
    { 0.945098,        0,        0},
    { 0.929412,        0,        0},
    { 0.913725,        0,        0},
    { 0.894118,        0,        0},
    { 0.878431,        0,        0},
    { 0.858824,        0,        0},
    { 0.843137,        0,        0},
    { 0.827451,        0,        0},
    { 0.807843,        0,        0},
    { 0.792157,        0,        0},
    { 0.772549,        0,        0},
    { 0.756863,        0,        0},
    { 0.741176,        0,        0},
    { 0.721569,        0,        0},
    { 0.705882,        0,        0},
    { 0.686275,        0,        0},
    { 0.670588,        0,        0},
    { 0.654902,        0,        0},
    { 0.635294,        0,        0},
    { 0.619608,        0,        0},
    {      0.6,        0,        0},
    { 0.584314,        0,        0},
    { 0.568627,        0,        0},
    {  0.54902,        0,        0},
    { 0.533333,        0,        0},
    { 0.513726,        0,        0},
    { 0.513726,        0,        0},
    { 0.513726,        0,        0},
    {       -1,       -1,       -1} };
/* ----------------------------------------------------------------------------
			The black and white color table
*/
char  blkwht_ct_name[] = {"Black/White"};
float blkwht_colortable[][3] = {
   { 0.000000,  0.000000,  0.000000},
   { 0.003922,  0.003922,  0.003922},
   { 0.007843,  0.007843,  0.007843},
   { 0.011765,  0.011765,  0.011765},
   { 0.015686,  0.015686,  0.015686},
   { 0.019608,  0.019608,  0.019608},
   { 0.023529,  0.023529,  0.023529},
   { 0.027451,  0.027451,  0.027451},
   { 0.031373,  0.031373,  0.031373},
   { 0.035294,  0.035294,  0.035294},
   { 0.039216,  0.039216,  0.039216},
   { 0.043137,  0.043137,  0.043137},
   { 0.047059,  0.047059,  0.047059},
   { 0.050980,  0.050980,  0.050980},
   { 0.054902,  0.054902,  0.054902},
   { 0.058824,  0.058824,  0.058824},
   { 0.062745,  0.062745,  0.062745},
   { 0.066667,  0.066667,  0.066667},
   { 0.070588,  0.070588,  0.070588},
   { 0.074510,  0.074510,  0.074510},
   { 0.078431,  0.078431,  0.078431},
   { 0.082353,  0.082353,  0.082353},
   { 0.086275,  0.086275,  0.086275},
   { 0.090196,  0.090196,  0.090196},
   { 0.094118,  0.094118,  0.094118},
   { 0.098039,  0.098039,  0.098039},
   { 0.101961,  0.101961,  0.101961},
   { 0.105882,  0.105882,  0.105882},
   { 0.109804,  0.109804,  0.109804},
   { 0.113725,  0.113725,  0.113725},
   { 0.117647,  0.117647,  0.117647},
   { 0.121569,  0.121569,  0.121569},
   { 0.125490,  0.125490,  0.125490},
   { 0.129412,  0.129412,  0.129412},
   { 0.133333,  0.133333,  0.133333},
   { 0.137255,  0.137255,  0.137255},
   { 0.141176,  0.141176,  0.141176},
   { 0.145098,  0.145098,  0.145098},
   { 0.149020,  0.149020,  0.149020},
   { 0.152941,  0.152941,  0.152941},
   { 0.156863,  0.156863,  0.156863},
   { 0.160784,  0.160784,  0.160784},
   { 0.164706,  0.164706,  0.164706},
   { 0.168627,  0.168627,  0.168627},
   { 0.172549,  0.172549,  0.172549},
   { 0.176471,  0.176471,  0.176471},
   { 0.180392,  0.180392,  0.180392},
   { 0.184314,  0.184314,  0.184314},
   { 0.188235,  0.188235,  0.188235},
   { 0.192157,  0.192157,  0.192157},
   { 0.196078,  0.196078,  0.196078},
   { 0.200000,  0.200000,  0.200000},
   { 0.203922,  0.203922,  0.203922},
   { 0.207843,  0.207843,  0.207843},
   { 0.211765,  0.211765,  0.211765},
   { 0.215686,  0.215686,  0.215686},
   { 0.219608,  0.219608,  0.219608},
   { 0.223529,  0.223529,  0.223529},
   { 0.227451,  0.227451,  0.227451},
   { 0.231373,  0.231373,  0.231373},
   { 0.235294,  0.235294,  0.235294},
   { 0.239216,  0.239216,  0.239216},
   { 0.243137,  0.243137,  0.243137},
   { 0.247059,  0.247059,  0.247059},
   { 0.250980,  0.250980,  0.250980},
   { 0.254902,  0.254902,  0.254902},
   { 0.258824,  0.258824,  0.258824},
   { 0.262745,  0.262745,  0.262745},
   { 0.266667,  0.266667,  0.266667},
   { 0.270588,  0.270588,  0.270588},
   { 0.274510,  0.274510,  0.274510},
   { 0.278431,  0.278431,  0.278431},
   { 0.282353,  0.282353,  0.282353},
   { 0.286275,  0.286275,  0.286275},
   { 0.290196,  0.290196,  0.290196},
   { 0.294118,  0.294118,  0.294118},
   { 0.298039,  0.298039,  0.298039},
   { 0.301961,  0.301961,  0.301961},
   { 0.305882,  0.305882,  0.305882},
   { 0.309804,  0.309804,  0.309804},
   { 0.313725,  0.313725,  0.313725},
   { 0.317647,  0.317647,  0.317647},
   { 0.321569,  0.321569,  0.321569},
   { 0.325490,  0.325490,  0.325490},
   { 0.329412,  0.329412,  0.329412},
   { 0.333333,  0.333333,  0.333333},
   { 0.337255,  0.337255,  0.337255},
   { 0.341176,  0.341176,  0.341176},
   { 0.345098,  0.345098,  0.345098},
   { 0.349020,  0.349020,  0.349020},
   { 0.352941,  0.352941,  0.352941},
   { 0.356863,  0.356863,  0.356863},
   { 0.360784,  0.360784,  0.360784},
   { 0.364706,  0.364706,  0.364706},
   { 0.368627,  0.368627,  0.368627},
   { 0.372549,  0.372549,  0.372549},
   { 0.376471,  0.376471,  0.376471},
   { 0.380392,  0.380392,  0.380392},
   { 0.384314,  0.384314,  0.384314},
   { 0.388235,  0.388235,  0.388235},
   { 0.392157,  0.392157,  0.392157},
   { 0.396078,  0.396078,  0.396078},
   { 0.400000,  0.400000,  0.400000},
   { 0.403922,  0.403922,  0.403922},
   { 0.407843,  0.407843,  0.407843},
   { 0.411765,  0.411765,  0.411765},
   { 0.415686,  0.415686,  0.415686},
   { 0.419608,  0.419608,  0.419608},
   { 0.423529,  0.423529,  0.423529},
   { 0.427451,  0.427451,  0.427451},
   { 0.431373,  0.431373,  0.431373},
   { 0.435294,  0.435294,  0.435294},
   { 0.439216,  0.439216,  0.439216},
   { 0.443137,  0.443137,  0.443137},
   { 0.447059,  0.447059,  0.447059},
   { 0.450980,  0.450980,  0.450980},
   { 0.454902,  0.454902,  0.454902},
   { 0.458824,  0.458824,  0.458824},
   { 0.462745,  0.462745,  0.462745},
   { 0.466667,  0.466667,  0.466667},
   { 0.470588,  0.470588,  0.470588},
   { 0.474510,  0.474510,  0.474510},
   { 0.478431,  0.478431,  0.478431},
   { 0.482353,  0.482353,  0.482353},
   { 0.486275,  0.486275,  0.486275},
   { 0.490196,  0.490196,  0.490196},
   { 0.494118,  0.494118,  0.494118},
   { 0.498039,  0.498039,  0.498039},
   { 0.501961,  0.501961,  0.501961},
   { 0.505882,  0.505882,  0.505882},
   { 0.509804,  0.509804,  0.509804},
   { 0.513725,  0.513725,  0.513725},
   { 0.517647,  0.517647,  0.517647},
   { 0.521569,  0.521569,  0.521569},
   { 0.525490,  0.525490,  0.525490},
   { 0.529412,  0.529412,  0.529412},
   { 0.533333,  0.533333,  0.533333},
   { 0.537255,  0.537255,  0.537255},
   { 0.541176,  0.541176,  0.541176},
   { 0.545098,  0.545098,  0.545098},
   { 0.549020,  0.549020,  0.549020},
   { 0.552941,  0.552941,  0.552941},
   { 0.556863,  0.556863,  0.556863},
   { 0.560784,  0.560784,  0.560784},
   { 0.564706,  0.564706,  0.564706},
   { 0.568627,  0.568627,  0.568627},
   { 0.572549,  0.572549,  0.572549},
   { 0.576471,  0.576471,  0.576471},
   { 0.580392,  0.580392,  0.580392},
   { 0.584314,  0.584314,  0.584314},
   { 0.588235,  0.588235,  0.588235},
   { 0.592157,  0.592157,  0.592157},
   { 0.596078,  0.596078,  0.596078},
   { 0.600000,  0.600000,  0.600000},
   { 0.603922,  0.603922,  0.603922},
   { 0.607843,  0.607843,  0.607843},
   { 0.611765,  0.611765,  0.611765},
   { 0.615686,  0.615686,  0.615686},
   { 0.619608,  0.619608,  0.619608},
   { 0.623529,  0.623529,  0.623529},
   { 0.627451,  0.627451,  0.627451},
   { 0.631373,  0.631373,  0.631373},
   { 0.635294,  0.635294,  0.635294},
   { 0.639216,  0.639216,  0.639216},
   { 0.643137,  0.643137,  0.643137},
   { 0.647059,  0.647059,  0.647059},
   { 0.650980,  0.650980,  0.650980},
   { 0.654902,  0.654902,  0.654902},
   { 0.658824,  0.658824,  0.658824},
   { 0.662745,  0.662745,  0.662745},
   { 0.666667,  0.666667,  0.666667},
   { 0.670588,  0.670588,  0.670588},
   { 0.674510,  0.674510,  0.674510},
   { 0.678431,  0.678431,  0.678431},
   { 0.682353,  0.682353,  0.682353},
   { 0.686275,  0.686275,  0.686275},
   { 0.690196,  0.690196,  0.690196},
   { 0.694118,  0.694118,  0.694118},
   { 0.698039,  0.698039,  0.698039},
   { 0.701961,  0.701961,  0.701961},
   { 0.705882,  0.705882,  0.705882},
   { 0.709804,  0.709804,  0.709804},
   { 0.713725,  0.713725,  0.713725},
   { 0.717647,  0.717647,  0.717647},
   { 0.721569,  0.721569,  0.721569},
   { 0.725490,  0.725490,  0.725490},
   { 0.729412,  0.729412,  0.729412},
   { 0.733333,  0.733333,  0.733333},
   { 0.737255,  0.737255,  0.737255},
   { 0.741176,  0.741176,  0.741176},
   { 0.745098,  0.745098,  0.745098},
   { 0.749020,  0.749020,  0.749020},
   { 0.752941,  0.752941,  0.752941},
   { 0.756863,  0.756863,  0.756863},
   { 0.760784,  0.760784,  0.760784},
   { 0.764706,  0.764706,  0.764706},
   { 0.768627,  0.768627,  0.768627},
   { 0.772549,  0.772549,  0.772549},
   { 0.776471,  0.776471,  0.776471},
   { 0.780392,  0.780392,  0.780392},
   { 0.784314,  0.784314,  0.784314},
   { 0.788235,  0.788235,  0.788235},
   { 0.792157,  0.792157,  0.792157},
   { 0.796078,  0.796078,  0.796078},
   { 0.800000,  0.800000,  0.800000},
   { 0.803922,  0.803922,  0.803922},
   { 0.807843,  0.807843,  0.807843},
   { 0.811765,  0.811765,  0.811765},
   { 0.815686,  0.815686,  0.815686},
   { 0.819608,  0.819608,  0.819608},
   { 0.823529,  0.823529,  0.823529},
   { 0.827451,  0.827451,  0.827451},
   { 0.831373,  0.831373,  0.831373},
   { 0.835294,  0.835294,  0.835294},
   { 0.839216,  0.839216,  0.839216},
   { 0.843137,  0.843137,  0.843137},
   { 0.847059,  0.847059,  0.847059},
   { 0.850980,  0.850980,  0.850980},
   { 0.854902,  0.854902,  0.854902},
   { 0.858824,  0.858824,  0.858824},
   { 0.862745,  0.862745,  0.862745},
   { 0.866667,  0.866667,  0.866667},
   { 0.870588,  0.870588,  0.870588},
   { 0.874510,  0.874510,  0.874510},
   { 0.878431,  0.878431,  0.878431},
   { 0.882353,  0.882353,  0.882353},
   { 0.886275,  0.886275,  0.886275},
   { 0.890196,  0.890196,  0.890196},
   { 0.894118,  0.894118,  0.894118},
   { 0.898039,  0.898039,  0.898039},
   { 0.901961,  0.901961,  0.901961},
   { 0.905882,  0.905882,  0.905882},
   { 0.909804,  0.909804,  0.909804},
   { 0.913725,  0.913725,  0.913725},
   { 0.917647,  0.917647,  0.917647},
   { 0.921569,  0.921569,  0.921569},
   { 0.925490,  0.925490,  0.925490},
   { 0.929412,  0.929412,  0.929412},
   { 0.933333,  0.933333,  0.933333},
   { 0.937255,  0.937255,  0.937255},
   { 0.941176,  0.941176,  0.941176},
   { 0.945098,  0.945098,  0.945098},
   { 0.949020,  0.949020,  0.949020},
   { 0.952941,  0.952941,  0.952941},
   { 0.956863,  0.956863,  0.956863},
   { 0.960784,  0.960784,  0.960784},
   { 0.964706,  0.964706,  0.964706},
   { 0.968627,  0.968627,  0.968627},
   { 0.972549,  0.972549,  0.972549},
   { 0.976471,  0.976471,  0.976471},
   { 0.980392,  0.980392,  0.980392},
   { 0.984314,  0.984314,  0.984314},
   { 0.988235,  0.988235,  0.988235},
   { 0.992157,  0.992157,  0.992157},
   { 0.996078,  0.996078,  0.996078},
   { 1.000000,  1.000000,  1.000000},
   {-1.000000, -1.000000, -1.000000} };
#endif
