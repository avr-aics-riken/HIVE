FilterImage = {}
setmetatable(FilterImage, {__index = HiveBaseModule})

FilterImage.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=FilterImage});
    this.filter = ImageFilter()
    this.res = 0;
    return this
end

function FilterImage:Do()
    self:UpdateValue()
    local v = self.value;
    local image0 = v.image0;
    local image1 = v.image1;
    local ope = v.operation;
    local opacity = v.opacity;
    if image0 ~= nil and image1 ~= nil then
        self.res = self.filter:Filter(ope, image0, image1, opacity);
    end
    return true
end

function FilterImage:result()
    return self.res;
end
