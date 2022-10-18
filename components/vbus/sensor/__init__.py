import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from .. import vbus_ns, VBus, CONF_VBUS_ID

DeltaSol_C = vbus_ns.class_('DeltaSol_C', cg.Component)
DeltaSol_BS_Plus = vbus_ns.class_('DeltaSol_BS_Plus', cg.Component)
VBusCustom = vbus_ns.class_('VBusCustom', cg.Component)

VBusModel = vbus_ns.enum("VBusModel")

MODELS = {
    'DELTASOL C': VBusModel.DELTASOL_C,
    'DELTASOL BS PLUS.4': VBusModel.DELTASOL_BS_PLUS,
}

VBUS_MODEL = cv.enum(MODELS, upper=True, space="_")

CONF_DELTASOL_C = "deltasol_c"
CONF_DELTASOL_BS_PLUS = "deltasol_bs_plus"
CONF_CUSTOM = "custom"

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_DELTASOL_C: cv.COMPONENT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(DeltaSol_C),
                cv.GenerateID(CONF_VBUS_ID): cv.use_id(VBus),
                
            }
        )
    },
    lower=True, space="_",
)

async def to_code(config):
    vbus = await cg.get_variable(config[CONF_VBUS_ID])

