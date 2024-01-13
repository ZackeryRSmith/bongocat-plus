--==========================--
-- Just testing lua support --
--==========================--

-- laad our images. LOAD BEFORE CREATING THE WINDOW!
local cat = BongoSprite.loadFromFile("cats/hard_worker/cat.png")
local eyes1 = BongoSprite.loadFromFile("cats/hard_worker/eyes1.png")
local eyes2 = BongoSprite.loadFromFile("cats/hard_worker/eyes2.png")
local left_down = BongoSprite.loadFromFile("cats/hard_worker/left_down.png")
local left_up = BongoSprite.loadFromFile("cats/hard_worker/left_up.png")
local right_down = BongoSprite.loadFromFile("cats/hard_worker/right_down.png")
local right_up = BongoSprite.loadFromFile("cats/hard_worker/right_up.png")

-- states
local left_state = false
local right_state = false

BongoWindow.create()

while BongoWindow.processEvents() == 0 do
	BongoWindow.clear(Sfml.Color.White)

	-- state logic
	-- (I want to have it so one half of the keyboard does left and the other does right)
	left_state = isPressed("z")
	right_state = isPressed("x")

	-- draw the bg sprite to the window
	BongoWindow.draw(cat)
	BongoWindow.draw(eyes1)

	-- draw ifelse logic
	BongoWindow.drawifelse(left_down, left_state, left_up)
	BongoWindow.drawifelse(right_down, right_state, right_up)

	-- display the window's contents
	BongoWindow.display()
end
