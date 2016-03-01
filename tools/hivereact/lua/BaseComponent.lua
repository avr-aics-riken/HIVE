
local BaseComponent = {}

BaseComponent.new = function (varname)
    local this = {}
    this.varname = varname
    this.property = {}
    this.connection = {}
    setmetatable(this, {__index = BaseComponent})
    return this;
end

function BaseComponent:ClearConnect()
    self.connection = {}
end

local function checkArray(name)
      local p = string.find(name, "%[%d+%]")
      if p == nil then
         return false
      end
      local arrayName = string.sub(name, 0, p-1)
      local idx = string.sub(name, p+1, -2)
      return true, arrayName, idx
end

function BaseComponent:Connect(propname, value)
    local r, arrayname, index
    r, arrayname, index = checkArray(propname)
    if r == false then
        self.connection[propname] = value
    else
        local carr = self.connection[arrayname] 
        if carr then
            carr[#carr + 1] = value
        else
            self.connection[arrayname] = {value}
        end     
    end        
end

function BaseComponent:Set(propname, value)
    self.property[propname] = value
end

function BaseComponent:Do()    
    print('[Warning:BaseComponent][LUA] Need to override Do() method.')
end

return BaseComponent