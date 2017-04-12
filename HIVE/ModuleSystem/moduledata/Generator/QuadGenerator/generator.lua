
QuadGenerator = {}
setmetatable(QuadGenerator, {__index = HiveBaseModule})

QuadGenerator.new = function (varname)
    local this = HiveBaseModule.new(varname)
    this.gen = PrimitiveGenerator()
    setmetatable(this, {__index=QuadGenerator})
    return this
end

function QuadGenerator:Do()
    self:UpdateValue()
    local v = self.value
    local width = v.width
	local height = v.height
    self.mesh = self.gen:Quad(width, height);
    return (self.point ~= nil);
end

function QuadGenerator:MeshData()
    return self.mesh;
end
