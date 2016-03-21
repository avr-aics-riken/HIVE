
PointGenerator = {}
setmetatable(PointGenerator, {__index = HiveBaseModule})

PointGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=PointGenerator})

    return this
end

function PointGenerator:Do()
    self:UpdateValue()

	--[[
	self.value.vertices = {}
	for i = 1, 10 do
		table.insert(self.value.vertices, 100 * math.sin(i))
		table.insert(self.value.vertices, 100 * math.cos(i))
		table.insert(self.value.vertices, 3 * math.cos(i) * math.sin(i))
	end
	]]

    return true
end

function PointGenerator:PointData()
    local v = self.value
    return self.gen:PointList(v.vertices, #v.vertices / 3, v.radius);
end
