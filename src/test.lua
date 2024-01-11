--==========================--
-- Just testing lua support --
--==========================--

-- laad our images. LOAD BEFORE CREATING THE WINDOW!
local cat = BongoSprite.loadFromFile("cats/original/cat.png")
local mouth_shut = BongoSprite.loadFromFile("cats/original/mouth_shut.png")
local mouth_open = BongoSprite.loadFromFile("cats/original/mouth_open.png")
local left_down = BongoSprite.loadFromFile("cats/original/left_down.png")
local left_up = BongoSprite.loadFromFile("cats/original/left_up.png")
local right_down = BongoSprite.loadFromFile("cats/original/right_down.png")
local right_up = BongoSprite.loadFromFile("cats/original/right_up.png")

-- states
local left_state = false
local right_state = false

BongoWindow.create(800, 450)

while BongoWindow.processEvents() == 0 do
	BongoWindow.clear(Sfml.Color.White)

	-- state logic
	left_state = isPressed("z")
	right_state = isPressed("x")

	-- draw the bg sprite to the window
	BongoWindow.draw(cat)

	-- draw ifelse logic
	BongoWindow.drawifelse(mouth_open, left_state and right_state, mouth_shut)
	BongoWindow.drawifelse(left_down, left_state, left_up)
	BongoWindow.drawifelse(right_down, right_state, right_up)

	-- display the window's contents
	BongoWindow.display()
end
