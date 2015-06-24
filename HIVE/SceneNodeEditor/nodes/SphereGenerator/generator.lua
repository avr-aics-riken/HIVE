function SphereGenerator(property)
	local size = property.size
	return {
		PointData = function ()
			local gen  = PrimitiveGenerator()
			return gen:Sphere(size)
		end
	}
end
