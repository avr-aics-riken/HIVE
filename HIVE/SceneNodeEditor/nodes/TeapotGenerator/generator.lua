function TeapotGenerator(property)
	local size = property.size

	return {
		MeshData = function ()
			local gen  = PrimitiveGenerator()
			return gen:Teapot(size)
		end
	}
end
