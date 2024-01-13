--[[
    ========================================================================
    BongoCat+ Lua API Stubs
    ------------------------------------------------------------------------

    Purpose:
    This file provides a skeletal representation of the Lua API that is
    bound from C++ using LuaBridge. It doesn't contain actual 
    implementations but mimics the structure and signatures of the 
    expected Lua-bound functions, namespaces, and constants.

    Usage:
    - Development-time Aid: The primary purpose of this stub file is to 
      assist developers by providing auto-completion, linting, and type 
      hinting in Lua-supported IDEs. 
    - It ensures a clearer understanding of the available Lua API without 
      diving into the C++ binding code.
    - This file should NOT be loaded or executed at runtime as it only 
      serves as a development aid and has no functional implementations.

    Note:
    Remember to keep this file updated when the C++ to Lua bindings change 
    or are extended.

    ========================================================================
]]

BongoAudio = {}

function BongoAudio.loadSoundFromFile(path) end
function BongoAudio.loadMusicFromFile(path) end

-- ========================================================================
-- BongoWindow API Stubs
-- ========================================================================

BongoWindow = BongoWindow or {}

function BongoWindow.create(width, height, style) end
function BongoWindow.create(width, height, style, position) end

function BongoWindow.refresh() end
function BongoWindow.refresh(width, height, style) end
function BongoWindow.refresh(width, height, style, position) end

function BongoWindow.clear() end
function BongoWindow.clear(color) end

function BongoWindow.draw(drawable) end
function BongoWindow.drawif(drawable, bool) end
function BongoWindow.drawifelse(drawable, bool, drawable) end

function BongoWindow.display() end

function BongoWindow.processEvents() end

function BongoWindow.close() end

-- ========================================================================
-- BongoSprite API Stubs
-- ========================================================================
BongoSprite = BongoSprite or {}

function BongoSprite.loadFromFile(filename) end

-- ========================================================================
-- BongoInput API Stubs
-- ========================================================================

function isPressed(int)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end
function isPressed(char)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end

function onPressed(int)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end
function onPressed(char)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end

function onPeleased(int)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end
function onPeleased(char)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end

BongoInput = {}
function BongoInput.Mouse.positionOnScreen()
	return 0.0, 0.0
end
function BongoInput.Mouse.positionOnFocusedWindow()
	return 0.0, 0.0
end
function BongoInput.Mouse.positionOnHoveredWindow()
	return 0.0, 0.0
end

BongoInput.Key = {
	-- INFO: based off http://www.foreui.com/articles/Key_Code_Table.htm
	-- NOTE: 0 - 49
	backspace = 8,
	tab = 9,
	enter = 13,
	shift = 16,
	ctrl = 27,
	alt = 18,
	pause = 19,
	caps_lock = 20,
	escape = 27,
	page_up = 33,
	page_down = 34,
	-- NOTE: ommited as `end` is a lua keyword
	--end = 35,
	home = 36,
	left = 37,
	up = 38,
	right = 39,
	down = 40,
	insert = 45,
	delete = 46,
	zero = 48,
	one = 49,
	-- NOTE: 50 - 99
	two = 50,
	three = 51,
	four = 52,
	five = 53,
	six = 54,
	seven = 55,
	eight = 56,
	nine = 57,
	semicolon = 59,
	colon = 16 | 59,
	equals = 61,
	plus = 16 | 61,
	a = 65,
	b = 66,
	c = 67,
	d = 68,
	e = 69,
	f = 70,
	g = 71,
	h = 72,
	i = 73,
	j = 74,
	k = 75,
	l = 76,
	m = 77,
	n = 78,
	o = 79,
	p = 80,
	q = 81,
	r = 82,
	s = 83,
	t = 84,
	u = 85,
	v = 86,
	w = 87,
	x = 88,
	y = 89,
	z = 90,
	power = 91,
	right_click = 93,
	zero_numlock = 96,
	one_numlock = 97,
	two_numlock = 98,
	three_numlock = 99,
	-- NOTE: 100 - 149
	four_numlock = 100,
	five_numlock = 101,
	six_numlock = 102,
	seven_numlock = 103,
	eight_numlock = 104,
	nine_numlock = 105,
	asterix_numlock = 106,
	plus_numlock = 107,
	minus_numlock = 109,
	period_numlock = 110,
	slash_numlock = 111,
	F1 = 112,
	F2 = 113,
	F3 = 114,
	F4 = 115,
	F5 = 116,
	F6 = 117,
	F7 = 118,
	F8 = 119,
	F9 = 120,
	F10 = 121,
	F11 = 122,
	F12 = 123,
	num_lock = 144,
	scroll_lock = 145,
	-- NOTE: 150 - 199
	comma = 188,
	less_then = 16 | 188,
	period = 190,
	greater_then = 16 | 190,
	slash = 191,
	question_mark = 16 | 191,
	tick = 192,
	tilde = 16 | 192,
	-- NOTE: 200 - 249
	-- GET NAMES FOR THESE
	lbracket = 219,
	lbrace = 16 | 219,
	backslash = 220,
	pipe = 16 | 220,
	rbracket = 221,
	rbrace = 16 | 221,
	single_quote = 222,
	double_quote = 16 | 222,
}

-- ========================================================================
-- BongoHelper API Stubs
-- ========================================================================

function ternary(condition, if_true, if_false)
	return true -- just to ignore LSP errors the actual impl. is done in c++
end

-- logging
function bongoDebug(msg) end
function bongoError(msg) end
function bongoFatal(msg) end
function bongoInfo(msg) end
function bongoWarn(msg) end

BongoHelper = {}

--function BongoHelper.draw_circle(x, y, radius, color) end
--function BongoHelper.draw_line(x1, y1, x2, y2, color, thickness) end
--function BongoHelper.draw_arc(x1, y1, x2, y2, width, color) end
-- sx, sy ~ start x, y
-- px, py ~ peak x, y
-- ex, ey ~ end x, y
--function BongoHelper.draw_parabola(sx, sy, px, py, ex, ey, num_points) end

-- ========================================================================
-- SFML (Sfml namespace) API Stubs
-- ========================================================================

Sfml = {}

---------------------------------------------------------------------------
-- Vector2i
---------------------------------------------------------------------------
Sfml.Vector2i = {
	x = 0,
	y = 0,
	-- Constructors
	new = function(x, y)
		return setmetatable({}, Sfml.Vector2i)
	end,
}

---------------------------------------------------------------------------
-- Vector2f
---------------------------------------------------------------------------
Sfml.Vector2f = {
	x = 0.0,
	y = 0.0,
	-- Constructors
	new = function(x, y)
		return setmetatable({}, Sfml.Vector2f)
	end,
}

---------------------------------------------------------------------------
-- Vector2u
---------------------------------------------------------------------------
Sfml.Vector2u = {
	x = 0,
	y = 0,
	-- Constructors
	new = function(x, y)
		return setmetatable({}, Sfml.Vector2u)
	end,
}

---------------------------------------------------------------------------
-- IntRect
---------------------------------------------------------------------------
Sfml.IntRect = {
	left = 0,
	top = 0,
	width = 0,
	height = 0,
	-- Methods
	contains = function(self, x, y)
		return true
	end,
	intersects = function(self, rect)
		return true
	end,
	getPosition = function(self)
		return Sfml.Vector2i.new(0, 0)
	end,
	getSize = function(self)
		return Sfml.Vector2u.new(0, 0)
	end,
}

---------------------------------------------------------------------------
-- FloatRect
---------------------------------------------------------------------------
Sfml.FloatRect = {
	left = 0.0,
	top = 0.0,
	width = 0.0,
	height = 0.0,
	-- Methods
	contains = function(self, x, y)
		return true
	end,
	intersects = function(self, rect)
		return true
	end,
	getPosition = function(self)
		return Sfml.Vector2f.new(0, 0)
	end,
	getSize = function(self)
		return Sfml.Vector2f.new(0, 0)
	end,
}

---------------------------------------------------------------------------
-- Color
---------------------------------------------------------------------------
Sfml.Color = {
	r = 0,
	g = 0,
	b = 0,
	a = 0,
	-- Static Colors
	Black = Sfml.Color.new(0, 0, 0),
	White = Sfml.Color.new(255, 255, 255),
	Red = Sfml.Color.new(255, 0, 0),
	Green = Sfml.Color.new(0, 255, 0),
	Blue = Sfml.Color.new(0, 0, 255),
	Yellow = Sfml.Color.new(255, 255, 0),
	Magenta = Sfml.Color.new(255, 0, 255),
	Cyan = Sfml.Color.new(0, 255, 255),
	Transparent = Sfml.Color.new(0, 0, 0, 0),
}

---------------------------------------------------------------------------
-- Transform
---------------------------------------------------------------------------
Sfml.Transform = {
	-- Constructors and methods will be filled in later
}

---------------------------------------------------------------------------
-- Image
---------------------------------------------------------------------------
Sfml.Image = {
	-- Constructors and relevant methods will be filled in later
}

---------------------------------------------------------------------------
-- Texture
---------------------------------------------------------------------------
Sfml.Texture = {
	-- Constructors and relevant methods will be filled in later
}

---------------------------------------------------------------------------
-- Sprite
---------------------------------------------------------------------------
Sfml.Sprite = {
	-- Constructors and relevant methods will be filled in later
}

Sfml.Sound = {
	-- Constructors and relevant methods will be filled in later
}

Sfml.SoundBuffer = {
	-- Constructors and relevant methods will be filled in later
}

Sfml.Music = {
	-- Constructors and relevant methods will be filled in later
}

---------------------------------------------------------------------------
-- Style
---------------------------------------------------------------------------
Sfml.Style = {
	None = 0,
	Titlebar = 1,
	Resize = 2,
	Close = 4,
	Fullscreen = 8,
	Default = 15, -- This is Titlebar + Resize + Close
}

return Sfml
