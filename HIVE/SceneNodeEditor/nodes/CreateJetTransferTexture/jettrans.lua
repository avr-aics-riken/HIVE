function CreateJetTransferTexture(property)

   -- Create 1D transfer function texture
   -- Jet color table from OSPray example
   -- r, g, b, alpha
   local tf = 
        { 0         , 0           , 0.562493 , 1.0
        , 0         , 0           , 1        , 1.0
        , 0         , 1           , 1        , 1.0
        , 0.500008  , 1           , 0.500008 , 1.0
        , 1         , 1           , 0        , 1.0
        , 1         , 0           , 0        , 1.0
        , 0.500008  , 0           , 0        , 1.0 }

 	local width = #tf / 4
	local tfTex = GenTexture()
	tfTex:Create2D(tf, 3, width, 1); -- 3 = RGBA32F

	return {
		Image = function ()
			return tfTex:ImageData()
		end
	}
end
