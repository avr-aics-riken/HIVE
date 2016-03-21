
PointLineGenerator = {}
setmetatable(PointLineGenerator, {__index = HiveBaseModule})

PointLineGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=PointLineGenerator})

    return this
end

function PointLineGenerator:Do()
    self:UpdateValue()

	self.value.vertices = {}
	for i = 1, 10 do
		table.insert(self.value.vertices, 100 * math.sin(i))
		table.insert(self.value.vertices, 100 * math.cos(i))
		table.insert(self.value.vertices, 3 * math.cos(i) * math.sin(i))
	end

    return true
end

function PointLineGenerator:PointData()
    local v = self.value
	print(self.gen)
    return self.gen:PointList(v.vertices, #v.vertices / 3, v.radius);
end

function PointLineGenerator:LineData()
    local v = self.value
    return self.gen:LineList(v.vertices, #v.vertices / 3, v.radius);
end
