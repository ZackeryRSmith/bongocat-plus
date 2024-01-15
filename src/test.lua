--==========================--
-- Just testing lua support --
--==========================----------------------------
-- For anyone reading this... my code for drawing the --
-- paw is poor. Please ignore this as it'll be fixed  --
-- before this cat comes out (into production)        --
--------------------------------------------------------

-- laad our images. LOAD BEFORE CREATING THE WINDOW!
local cat = BongoSprite.loadFromFile("cats/keyboard/cat.png")

-- keys
local k_1 = BongoSprite.loadFromFile("cats/keyboard/keys/1.png")
local k_2 = BongoSprite.loadFromFile("cats/keyboard/keys/2.png")
local k_3 = BongoSprite.loadFromFile("cats/keyboard/keys/3.png")
local k_4 = BongoSprite.loadFromFile("cats/keyboard/keys/4.png")
local k_5 = BongoSprite.loadFromFile("cats/keyboard/keys/5.png")
local k_6 = BongoSprite.loadFromFile("cats/keyboard/keys/6.png")
local k_7 = BongoSprite.loadFromFile("cats/keyboard/keys/7.png")
local k_a = BongoSprite.loadFromFile("cats/keyboard/keys/a.png")
local k_d = BongoSprite.loadFromFile("cats/keyboard/keys/d.png")
local k_e = BongoSprite.loadFromFile("cats/keyboard/keys/e.png")
local k_q = BongoSprite.loadFromFile("cats/keyboard/keys/q.png")
local k_r = BongoSprite.loadFromFile("cats/keyboard/keys/r.png")
local k_s = BongoSprite.loadFromFile("cats/keyboard/keys/s.png")
local k_w = BongoSprite.loadFromFile("cats/keyboard/keys/w.png")
local k_space = BongoSprite.loadFromFile("cats/keyboard/keys/space.png")

-- paws
local p_up = BongoSprite.loadFromFile("cats/keyboard/paw/leftup.png")
local p_1 = BongoSprite.loadFromFile("cats/keyboard/paw/1.png")
local p_2 = BongoSprite.loadFromFile("cats/keyboard/paw/2.png")
local p_3 = BongoSprite.loadFromFile("cats/keyboard/paw/3.png")
local p_4 = BongoSprite.loadFromFile("cats/keyboard/paw/4.png")
local p_5 = BongoSprite.loadFromFile("cats/keyboard/paw/5.png")
local p_6 = BongoSprite.loadFromFile("cats/keyboard/paw/6.png")
local p_7 = BongoSprite.loadFromFile("cats/keyboard/paw/7.png")
local p_a = BongoSprite.loadFromFile("cats/keyboard/paw/a.png")
local p_d = BongoSprite.loadFromFile("cats/keyboard/paw/d.png")
local p_e = BongoSprite.loadFromFile("cats/keyboard/paw/e.png")
local p_q = BongoSprite.loadFromFile("cats/keyboard/paw/q.png")
local p_r = BongoSprite.loadFromFile("cats/keyboard/paw/r.png")
local p_s = BongoSprite.loadFromFile("cats/keyboard/paw/s.png")
local p_w = BongoSprite.loadFromFile("cats/keyboard/paw/w.png")
local p_space = BongoSprite.loadFromFile("cats/keyboard/paw/space.png")

local current_pressed_keys = {}
local previous_pressed_keys = {}
local draw_key = BongoInput.Key.None

local function allFalse(t)
	for _, v in pairs(t) do
		if v then
			return false
		end
	end

	return true
end

BongoWindow.create()

while BongoWindow.processEvents() == 0 do
	BongoWindow.clear(Sfml.Color.White)

	-- draw the bg sprite to the window
	BongoWindow.draw(cat)

	-- draw left hand as up if no keys are pressed

	-- draw key state
	BongoWindow.drawif(k_1, isPressed(BongoInput.Key.one))
	BongoWindow.drawif(k_2, isPressed(BongoInput.Key.two))
	BongoWindow.drawif(k_3, isPressed(BongoInput.Key.three))
	BongoWindow.drawif(k_4, isPressed(BongoInput.Key.four))
	BongoWindow.drawif(k_5, isPressed(BongoInput.Key.five))
	BongoWindow.drawif(k_6, isPressed(BongoInput.Key.six))
	BongoWindow.drawif(k_7, isPressed(BongoInput.Key.seven))
	BongoWindow.drawif(k_a, isPressed(BongoInput.Key.a))
	BongoWindow.drawif(k_d, isPressed(BongoInput.Key.d))
	BongoWindow.drawif(k_e, isPressed(BongoInput.Key.e))
	BongoWindow.drawif(k_q, isPressed(BongoInput.Key.q))
	BongoWindow.drawif(k_r, isPressed(BongoInput.Key.r))
	BongoWindow.drawif(k_s, isPressed(BongoInput.Key.s))
	BongoWindow.drawif(k_w, isPressed(BongoInput.Key.w))
	BongoWindow.drawif(k_space, isPressed(BongoInput.Key.space))

	-- Update the current array of pressed keys
	for k = 1, BongoInput.KeyCount - 1, 1 do
		current_pressed_keys[k] = isPressed(k)
	end

	-- find the most recent pressed key by comparing with the previous state
	for k = 1, BongoInput.KeyCount - 1, 1 do
		if current_pressed_keys[k] and not previous_pressed_keys[k] then
			draw_key = k
			break
		end
	end

	-- if the recent key is dropped, and no new keys are pressed
	if previous_pressed_keys[draw_key] and not current_pressed_keys[draw_key] then
		for k, value in pairs(current_pressed_keys) do
			if value then
				draw_key = k
				break
			end
		end
	end

	-- if all keys are down then draw_key is None
	if allFalse(current_pressed_keys) then
		draw_key = BongoInput.Key.None
	end

	-- draw paw state
	BongoWindow.drawif(p_up, draw_key == BongoInput.Key.None)
	BongoWindow.drawif(p_1, draw_key == BongoInput.Key.one)
	BongoWindow.drawif(p_2, draw_key == BongoInput.Key.two)
	BongoWindow.drawif(p_3, draw_key == BongoInput.Key.three)
	BongoWindow.drawif(p_4, draw_key == BongoInput.Key.four)
	BongoWindow.drawif(p_5, draw_key == BongoInput.Key.five)
	BongoWindow.drawif(p_6, draw_key == BongoInput.Key.six)
	BongoWindow.drawif(p_7, draw_key == BongoInput.Key.seven)
	BongoWindow.drawif(p_a, draw_key == BongoInput.Key.a)
	BongoWindow.drawif(p_d, draw_key == BongoInput.Key.d)
	BongoWindow.drawif(p_e, draw_key == BongoInput.Key.e)
	BongoWindow.drawif(p_q, draw_key == BongoInput.Key.q)
	BongoWindow.drawif(p_r, draw_key == BongoInput.Key.r)
	BongoWindow.drawif(p_s, draw_key == BongoInput.Key.s)
	BongoWindow.drawif(p_w, draw_key == BongoInput.Key.w)
	BongoWindow.drawif(p_space, draw_key == BongoInput.Key.space)

	for k = 1, BongoInput.KeyCount - 1, 1 do
		previous_pressed_keys[k] = current_pressed_keys[k]
	end

	-- display the window's contents
	BongoWindow.display()
end
