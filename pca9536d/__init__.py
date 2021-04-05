import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_NUMBER, CONF_MODE, CONF_INVERTED

DEPENDENCIES = ["i2c"]
MULTI_CONF = True

CONF_PCA9536D = "pca9536d"

pca9536d_ns = cg.esphome_ns.namespace("pca9536d")
PCA9536DGPIOMode = pca9536d_ns.enum("PCA9536DGPIOMode")
PCA9536D_GPIO_MODES = {
  "INPUT": PCA9536DGPIOMode.PCA9536D_INPUT,
  "OUTPUT": PCA9536DGPIOMode.PCA9536D_OUTPUT,
}

PCA9536DComponent = pca9536d_ns.class_("PCA9536DComponent", cg.Component, i2c.I2CDevice)
PCA9536DGPIOPin = pca9536d_ns.class_("PCA9536DGPIOPin", cg.GPIOPin)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(PCA9536DComponent),
  }).extend(i2c.i2c_device_schema(0x41))

def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])
  yield cg.register_component(var, config)
  yield i2c.register_i2c_device(var, config)

PCA9536D_OUTPUT_PIN_SCHEMA = cv.Schema({
  cv.Required(CONF_PCA9536D): cv.use_id(PCA9536DComponent),
  cv.Required(CONF_NUMBER): cv.int_,
  cv.Optional(CONF_MODE, default="OUTPUT"): cv.enum(PCA9536D_GPIO_MODES, upper=True),
  cv.Optional(CONF_INVERTED, default=False): cv.boolean,
})

PCA9536D_INPUT_PIN_SCHEMA = cv.Schema({
  cv.Required(CONF_PCA9536D): cv.use_id(PCA9536DComponent),
  cv.Required(CONF_NUMBER): cv.int_,
  cv.Optional(CONF_MODE, default="INPUT"): cv.enum(PCA9536D_GPIO_MODES, upper=True),
  cv.Optional(CONF_INVERTED, default=False): cv.boolean,
})


@pins.PIN_SCHEMA_REGISTRY.register(
  "pca9536d", (PCA9536D_OUTPUT_PIN_SCHEMA, PCA9536D_INPUT_PIN_SCHEMA)
)
def pca9536d_pin_to_code(config):
  parent = yield cg.get_variable(config[CONF_PCA9536D])
  yield PCA9536DGPIOPin.new(parent, config[CONF_NUMBER], config[CONF_MODE], config[CONF_INVERTED])
