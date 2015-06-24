function CubeGenerator(property)
	local size = {property.size[1], property.size[2], property.size[3]}

	return {
		MeshData = function ()
			local gen  = PrimitiveGenerator()
			return gen:Cube(size[1], size[2], size[3])
		end
	}
end
