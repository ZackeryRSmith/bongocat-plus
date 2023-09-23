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

function BongoWindow.display() end

function BongoWindow.processEvents() end

function BongoWindow.close() end

-- these are functions from SFML directly bound to BongoWindow

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
	-- ... (Add other static colors here)
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
