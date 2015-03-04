function QuadGenerator(property)
	local size = {property.size[1], property.size[2]}

	return {
		MeshData = function ()
			local gen  = PrimitiveGenerator()
			return gen:Quad(size[1], size[2])
		end
	}
end
