--==========================--
-- Just testing lua support --
--==========================--

-- create a window
BongoWindow.create(800, 450, Sfml.Style.Default)

-- laad an image into a texture
local texture = Sfml.Texture()
if not texture:loadFromFile("res/catbanner.jpg", Sfml.IntRect()) then
	print("Error loading image")
	return
end

-- create a sprite and set it's texture
local sprite = Sfml.Sprite()
sprite:setTexture(texture)

while BongoWindow.processEvents() == 0 do
	-- clear the window with a color (optional but recommended)
	BongoWindow.clear(Sfml.Color.Black)

	-- draw the sprite to the window
	BongoWindow.draw(sprite)

	-- display the window's contents
	BongoWindow.display()
end
