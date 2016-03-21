
LineGenerator = {}
setmetatable(LineGenerator, {__index = HiveBaseModule})

LineGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=LineGenerator})

    return this
end

function LineGenerator:Do()
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

function LineGenerator:LineData()
    local v = self.value
    return self.gen:LineList(v.vertices, #v.vertices / 3, v.radius);
end
