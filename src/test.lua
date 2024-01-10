--==========================--
-- Just testing lua support --
--==========================--

-- laad our images. LOAD BEFORE CREATING THE WINDOW!
local up = BongoSprite.loadFromFile("cats/osu/up.png")
local left = BongoSprite.loadFromFile("cats/osu/left.png")
local right = BongoSprite.loadFromFile("cats/osu/right.png")
local wave = BongoSprite.loadFromFile("cats/osu/wave.png")
local smoke = BongoSprite.loadFromFile("cats/osu/smoke.png")
local mousebg = BongoSprite.loadFromFile("cats/osu/mousebg.png")
local testbg = BongoSprite.loadFromFile("cats/osu/temp.png")

-- states
local up_state = false
local left_state = false
local right_state = false
local wave_state = false
local smoke_state = false

-- create a window
BongoWindow.create(612, 354)

while BongoWindow.processEvents() == 0 do
	-- clear the window with a color (optional but recommended)
	BongoWindow.clear(Sfml.Color.White)

	-- state logic
	left_state = isPressed(BongoInput.Key.z) and not right_state
	right_state = isPressed(BongoInput.Key.x) and not left_state
	wave_state = isPressed(BongoInput.Key.w)
	smoke_state = ternary(onPressed(BongoInput.Key.s), not smoke_state, smoke_state)
	up_state = not (left_state or right_state or wave_state)

	-- draw the bg sprite to the window
	BongoWindow.draw(mousebg)

	-- draw if logic
	BongoWindow.drawif(left, left_state)
	BongoWindow.drawif(right, right_state)
	BongoWindow.drawif(wave, wave_state)
	BongoWindow.drawif(smoke, smoke_state)
	BongoWindow.drawif(up, up_state)

	-- mouse hand logic
	local mdata = BongoInput.Mouse.positionOnHoveredWindow()
	local x, y = mdata[1], mdata[2]

	bongoDebug("(" .. x .. ", " .. y .. ")")

	-- display the window's contents
	BongoWindow.display()
end
