/*eslint-disable block-scoped-var, id-length, no-control-regex, no-magic-numbers, no-prototype-builtins, no-redeclare, no-shadow, no-var, sort-vars*/
(function(global, factory) { /* global define, require, module */

    /* AMD */ if (typeof define === 'function' && define.amd)
        define(["protobufjs/minimal"], factory);

    /* CommonJS */ else if (typeof require === 'function' && typeof module === 'object' && module && module.exports)
        module.exports = factory(require("protobufjs/minimal"));

})(this, function($protobuf) {
    "use strict";

    // Common aliases
    var $Reader = $protobuf.Reader, $Writer = $protobuf.Writer, $util = $protobuf.util;
    
    // Exported root namespace
    var $root = $protobuf.roots["default"] || ($protobuf.roots["default"] = {});
    
    $root.xtech = (function() {
    
        /**
         * Namespace xtech.
         * @exports xtech
         * @namespace
         */
        var xtech = {};
    
        xtech.selfomat = (function() {
    
            /**
             * Namespace selfomat.
             * @memberof xtech
             * @namespace
             */
            var selfomat = {};
    
            selfomat.Status = (function() {
    
                /**
                 * Properties of a Status.
                 * @memberof xtech.selfomat
                 * @interface IStatus
                 * @property {xtech.selfomat.Status.State} currentState Status currentState
                 */
    
                /**
                 * Constructs a new Status.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a Status.
                 * @implements IStatus
                 * @constructor
                 * @param {xtech.selfomat.IStatus=} [properties] Properties to set
                 */
                function Status(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * Status currentState.
                 * @member {xtech.selfomat.Status.State} currentState
                 * @memberof xtech.selfomat.Status
                 * @instance
                 */
                Status.prototype.currentState = 0;
    
                /**
                 * Creates a new Status instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {xtech.selfomat.IStatus=} [properties] Properties to set
                 * @returns {xtech.selfomat.Status} Status instance
                 */
                Status.create = function create(properties) {
                    return new Status(properties);
                };
    
                /**
                 * Encodes the specified Status message. Does not implicitly {@link xtech.selfomat.Status.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {xtech.selfomat.IStatus} message Status message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                Status.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.currentState);
                    return writer;
                };
    
                /**
                 * Encodes the specified Status message, length delimited. Does not implicitly {@link xtech.selfomat.Status.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {xtech.selfomat.IStatus} message Status message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                Status.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a Status message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.Status} Status
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                Status.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.Status();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.currentState = reader.int32();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("currentState"))
                        throw $util.ProtocolError("missing required 'currentState'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a Status message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.Status} Status
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                Status.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a Status message.
                 * @function verify
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                Status.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    switch (message.currentState) {
                    default:
                        return "currentState: enum value expected";
                    case 0:
                    case 1:
                    case 2:
                        break;
                    }
                    return null;
                };
    
                /**
                 * Creates a Status message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.Status} Status
                 */
                Status.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.Status)
                        return object;
                    var message = new $root.xtech.selfomat.Status();
                    switch (object.currentState) {
                    case "INITIALIZING":
                    case 0:
                        message.currentState = 0;
                        break;
                    case "RUNNING":
                    case 1:
                        message.currentState = 1;
                        break;
                    case "ERROR":
                    case 2:
                        message.currentState = 2;
                        break;
                    }
                    return message;
                };
    
                /**
                 * Creates a plain object from a Status message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.Status
                 * @static
                 * @param {xtech.selfomat.Status} message Status
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                Status.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults)
                        object.currentState = options.enums === String ? "INITIALIZING" : 0;
                    if (message.currentState != null && message.hasOwnProperty("currentState"))
                        object.currentState = options.enums === String ? $root.xtech.selfomat.Status.State[message.currentState] : message.currentState;
                    return object;
                };
    
                /**
                 * Converts this Status to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.Status
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                Status.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                /**
                 * State enum.
                 * @name xtech.selfomat.Status.State
                 * @enum {string}
                 * @property {number} INITIALIZING=0 INITIALIZING value
                 * @property {number} RUNNING=1 RUNNING value
                 * @property {number} ERROR=2 ERROR value
                 */
                Status.State = (function() {
                    var valuesById = {}, values = Object.create(valuesById);
                    values[valuesById[0] = "INITIALIZING"] = 0;
                    values[valuesById[1] = "RUNNING"] = 1;
                    values[valuesById[2] = "ERROR"] = 2;
                    return values;
                })();
    
                return Status;
            })();
    
            selfomat.IntUpdate = (function() {
    
                /**
                 * Properties of an IntUpdate.
                 * @memberof xtech.selfomat
                 * @interface IIntUpdate
                 * @property {number|Long} value IntUpdate value
                 */
    
                /**
                 * Constructs a new IntUpdate.
                 * @memberof xtech.selfomat
                 * @classdesc Represents an IntUpdate.
                 * @implements IIntUpdate
                 * @constructor
                 * @param {xtech.selfomat.IIntUpdate=} [properties] Properties to set
                 */
                function IntUpdate(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * IntUpdate value.
                 * @member {number|Long} value
                 * @memberof xtech.selfomat.IntUpdate
                 * @instance
                 */
                IntUpdate.prototype.value = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
                /**
                 * Creates a new IntUpdate instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {xtech.selfomat.IIntUpdate=} [properties] Properties to set
                 * @returns {xtech.selfomat.IntUpdate} IntUpdate instance
                 */
                IntUpdate.create = function create(properties) {
                    return new IntUpdate(properties);
                };
    
                /**
                 * Encodes the specified IntUpdate message. Does not implicitly {@link xtech.selfomat.IntUpdate.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {xtech.selfomat.IIntUpdate} message IntUpdate message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                IntUpdate.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 0 =*/8).int64(message.value);
                    return writer;
                };
    
                /**
                 * Encodes the specified IntUpdate message, length delimited. Does not implicitly {@link xtech.selfomat.IntUpdate.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {xtech.selfomat.IIntUpdate} message IntUpdate message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                IntUpdate.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes an IntUpdate message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.IntUpdate} IntUpdate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                IntUpdate.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.IntUpdate();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.value = reader.int64();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("value"))
                        throw $util.ProtocolError("missing required 'value'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes an IntUpdate message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.IntUpdate} IntUpdate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                IntUpdate.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies an IntUpdate message.
                 * @function verify
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                IntUpdate.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isInteger(message.value) && !(message.value && $util.isInteger(message.value.low) && $util.isInteger(message.value.high)))
                        return "value: integer|Long expected";
                    return null;
                };
    
                /**
                 * Creates an IntUpdate message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.IntUpdate} IntUpdate
                 */
                IntUpdate.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.IntUpdate)
                        return object;
                    var message = new $root.xtech.selfomat.IntUpdate();
                    if (object.value != null)
                        if ($util.Long)
                            (message.value = $util.Long.fromValue(object.value)).unsigned = false;
                        else if (typeof object.value === "string")
                            message.value = parseInt(object.value, 10);
                        else if (typeof object.value === "number")
                            message.value = object.value;
                        else if (typeof object.value === "object")
                            message.value = new $util.LongBits(object.value.low >>> 0, object.value.high >>> 0).toNumber();
                    return message;
                };
    
                /**
                 * Creates a plain object from an IntUpdate message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.IntUpdate
                 * @static
                 * @param {xtech.selfomat.IntUpdate} message IntUpdate
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                IntUpdate.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults)
                        if ($util.Long) {
                            var long = new $util.Long(0, 0, false);
                            object.value = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                        } else
                            object.value = options.longs === String ? "0" : 0;
                    if (message.value != null && message.hasOwnProperty("value"))
                        if (typeof message.value === "number")
                            object.value = options.longs === String ? String(message.value) : message.value;
                        else
                            object.value = options.longs === String ? $util.Long.prototype.toString.call(message.value) : options.longs === Number ? new $util.LongBits(message.value.low >>> 0, message.value.high >>> 0).toNumber() : message.value;
                    return object;
                };
    
                /**
                 * Converts this IntUpdate to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.IntUpdate
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                IntUpdate.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return IntUpdate;
            })();
    
            selfomat.BoolUpdate = (function() {
    
                /**
                 * Properties of a BoolUpdate.
                 * @memberof xtech.selfomat
                 * @interface IBoolUpdate
                 * @property {boolean} value BoolUpdate value
                 */
    
                /**
                 * Constructs a new BoolUpdate.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a BoolUpdate.
                 * @implements IBoolUpdate
                 * @constructor
                 * @param {xtech.selfomat.IBoolUpdate=} [properties] Properties to set
                 */
                function BoolUpdate(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * BoolUpdate value.
                 * @member {boolean} value
                 * @memberof xtech.selfomat.BoolUpdate
                 * @instance
                 */
                BoolUpdate.prototype.value = false;
    
                /**
                 * Creates a new BoolUpdate instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {xtech.selfomat.IBoolUpdate=} [properties] Properties to set
                 * @returns {xtech.selfomat.BoolUpdate} BoolUpdate instance
                 */
                BoolUpdate.create = function create(properties) {
                    return new BoolUpdate(properties);
                };
    
                /**
                 * Encodes the specified BoolUpdate message. Does not implicitly {@link xtech.selfomat.BoolUpdate.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {xtech.selfomat.IBoolUpdate} message BoolUpdate message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoolUpdate.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 0 =*/8).bool(message.value);
                    return writer;
                };
    
                /**
                 * Encodes the specified BoolUpdate message, length delimited. Does not implicitly {@link xtech.selfomat.BoolUpdate.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {xtech.selfomat.IBoolUpdate} message BoolUpdate message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoolUpdate.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a BoolUpdate message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.BoolUpdate} BoolUpdate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoolUpdate.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.BoolUpdate();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.value = reader.bool();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("value"))
                        throw $util.ProtocolError("missing required 'value'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a BoolUpdate message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.BoolUpdate} BoolUpdate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoolUpdate.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a BoolUpdate message.
                 * @function verify
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                BoolUpdate.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (typeof message.value !== "boolean")
                        return "value: boolean expected";
                    return null;
                };
    
                /**
                 * Creates a BoolUpdate message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.BoolUpdate} BoolUpdate
                 */
                BoolUpdate.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.BoolUpdate)
                        return object;
                    var message = new $root.xtech.selfomat.BoolUpdate();
                    if (object.value != null)
                        message.value = Boolean(object.value);
                    return message;
                };
    
                /**
                 * Creates a plain object from a BoolUpdate message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.BoolUpdate
                 * @static
                 * @param {xtech.selfomat.BoolUpdate} message BoolUpdate
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                BoolUpdate.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults)
                        object.value = false;
                    if (message.value != null && message.hasOwnProperty("value"))
                        object.value = message.value;
                    return object;
                };
    
                /**
                 * Converts this BoolUpdate to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.BoolUpdate
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                BoolUpdate.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return BoolUpdate;
            })();
    
            selfomat.FloatUpdate = (function() {
    
                /**
                 * Properties of a FloatUpdate.
                 * @memberof xtech.selfomat
                 * @interface IFloatUpdate
                 * @property {number} value FloatUpdate value
                 */
    
                /**
                 * Constructs a new FloatUpdate.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a FloatUpdate.
                 * @implements IFloatUpdate
                 * @constructor
                 * @param {xtech.selfomat.IFloatUpdate=} [properties] Properties to set
                 */
                function FloatUpdate(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * FloatUpdate value.
                 * @member {number} value
                 * @memberof xtech.selfomat.FloatUpdate
                 * @instance
                 */
                FloatUpdate.prototype.value = 0;
    
                /**
                 * Creates a new FloatUpdate instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {xtech.selfomat.IFloatUpdate=} [properties] Properties to set
                 * @returns {xtech.selfomat.FloatUpdate} FloatUpdate instance
                 */
                FloatUpdate.create = function create(properties) {
                    return new FloatUpdate(properties);
                };
    
                /**
                 * Encodes the specified FloatUpdate message. Does not implicitly {@link xtech.selfomat.FloatUpdate.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {xtech.selfomat.IFloatUpdate} message FloatUpdate message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                FloatUpdate.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 5 =*/13).float(message.value);
                    return writer;
                };
    
                /**
                 * Encodes the specified FloatUpdate message, length delimited. Does not implicitly {@link xtech.selfomat.FloatUpdate.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {xtech.selfomat.IFloatUpdate} message FloatUpdate message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                FloatUpdate.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a FloatUpdate message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.FloatUpdate} FloatUpdate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                FloatUpdate.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.FloatUpdate();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.value = reader.float();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("value"))
                        throw $util.ProtocolError("missing required 'value'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a FloatUpdate message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.FloatUpdate} FloatUpdate
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                FloatUpdate.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a FloatUpdate message.
                 * @function verify
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                FloatUpdate.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (typeof message.value !== "number")
                        return "value: number expected";
                    return null;
                };
    
                /**
                 * Creates a FloatUpdate message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.FloatUpdate} FloatUpdate
                 */
                FloatUpdate.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.FloatUpdate)
                        return object;
                    var message = new $root.xtech.selfomat.FloatUpdate();
                    if (object.value != null)
                        message.value = Number(object.value);
                    return message;
                };
    
                /**
                 * Creates a plain object from a FloatUpdate message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.FloatUpdate
                 * @static
                 * @param {xtech.selfomat.FloatUpdate} message FloatUpdate
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                FloatUpdate.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults)
                        object.value = 0;
                    if (message.value != null && message.hasOwnProperty("value"))
                        object.value = options.json && !isFinite(message.value) ? String(message.value) : message.value;
                    return object;
                };
    
                /**
                 * Converts this FloatUpdate to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.FloatUpdate
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                FloatUpdate.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return FloatUpdate;
            })();
    
            selfomat.ReadOnlySetting = (function() {
    
                /**
                 * Properties of a ReadOnlySetting.
                 * @memberof xtech.selfomat
                 * @interface IReadOnlySetting
                 * @property {string} name ReadOnlySetting name
                 * @property {string} value ReadOnlySetting value
                 */
    
                /**
                 * Constructs a new ReadOnlySetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a ReadOnlySetting.
                 * @implements IReadOnlySetting
                 * @constructor
                 * @param {xtech.selfomat.IReadOnlySetting=} [properties] Properties to set
                 */
                function ReadOnlySetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * ReadOnlySetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @instance
                 */
                ReadOnlySetting.prototype.name = "";
    
                /**
                 * ReadOnlySetting value.
                 * @member {string} value
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @instance
                 */
                ReadOnlySetting.prototype.value = "";
    
                /**
                 * Creates a new ReadOnlySetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {xtech.selfomat.IReadOnlySetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.ReadOnlySetting} ReadOnlySetting instance
                 */
                ReadOnlySetting.create = function create(properties) {
                    return new ReadOnlySetting(properties);
                };
    
                /**
                 * Encodes the specified ReadOnlySetting message. Does not implicitly {@link xtech.selfomat.ReadOnlySetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {xtech.selfomat.IReadOnlySetting} message ReadOnlySetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                ReadOnlySetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.value);
                    return writer;
                };
    
                /**
                 * Encodes the specified ReadOnlySetting message, length delimited. Does not implicitly {@link xtech.selfomat.ReadOnlySetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {xtech.selfomat.IReadOnlySetting} message ReadOnlySetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                ReadOnlySetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a ReadOnlySetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.ReadOnlySetting} ReadOnlySetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                ReadOnlySetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.ReadOnlySetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.value = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("value"))
                        throw $util.ProtocolError("missing required 'value'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a ReadOnlySetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.ReadOnlySetting} ReadOnlySetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                ReadOnlySetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a ReadOnlySetting message.
                 * @function verify
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                ReadOnlySetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (!$util.isString(message.value))
                        return "value: string expected";
                    return null;
                };
    
                /**
                 * Creates a ReadOnlySetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.ReadOnlySetting} ReadOnlySetting
                 */
                ReadOnlySetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.ReadOnlySetting)
                        return object;
                    var message = new $root.xtech.selfomat.ReadOnlySetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.value != null)
                        message.value = String(object.value);
                    return message;
                };
    
                /**
                 * Creates a plain object from a ReadOnlySetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @static
                 * @param {xtech.selfomat.ReadOnlySetting} message ReadOnlySetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                ReadOnlySetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        object.value = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.value != null && message.hasOwnProperty("value"))
                        object.value = message.value;
                    return object;
                };
    
                /**
                 * Converts this ReadOnlySetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.ReadOnlySetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                ReadOnlySetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return ReadOnlySetting;
            })();
    
            selfomat.PostSetting = (function() {
    
                /**
                 * Properties of a PostSetting.
                 * @memberof xtech.selfomat
                 * @interface IPostSetting
                 * @property {string} name PostSetting name
                 * @property {string} postUrl PostSetting postUrl
                 */
    
                /**
                 * Constructs a new PostSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a PostSetting.
                 * @implements IPostSetting
                 * @constructor
                 * @param {xtech.selfomat.IPostSetting=} [properties] Properties to set
                 */
                function PostSetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * PostSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.PostSetting
                 * @instance
                 */
                PostSetting.prototype.name = "";
    
                /**
                 * PostSetting postUrl.
                 * @member {string} postUrl
                 * @memberof xtech.selfomat.PostSetting
                 * @instance
                 */
                PostSetting.prototype.postUrl = "";
    
                /**
                 * Creates a new PostSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {xtech.selfomat.IPostSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.PostSetting} PostSetting instance
                 */
                PostSetting.create = function create(properties) {
                    return new PostSetting(properties);
                };
    
                /**
                 * Encodes the specified PostSetting message. Does not implicitly {@link xtech.selfomat.PostSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {xtech.selfomat.IPostSetting} message PostSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                PostSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.postUrl);
                    return writer;
                };
    
                /**
                 * Encodes the specified PostSetting message, length delimited. Does not implicitly {@link xtech.selfomat.PostSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {xtech.selfomat.IPostSetting} message PostSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                PostSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a PostSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.PostSetting} PostSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                PostSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.PostSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.postUrl = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("postUrl"))
                        throw $util.ProtocolError("missing required 'postUrl'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a PostSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.PostSetting} PostSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                PostSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a PostSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                PostSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (!$util.isString(message.postUrl))
                        return "postUrl: string expected";
                    return null;
                };
    
                /**
                 * Creates a PostSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.PostSetting} PostSetting
                 */
                PostSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.PostSetting)
                        return object;
                    var message = new $root.xtech.selfomat.PostSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.postUrl != null)
                        message.postUrl = String(object.postUrl);
                    return message;
                };
    
                /**
                 * Creates a plain object from a PostSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.PostSetting
                 * @static
                 * @param {xtech.selfomat.PostSetting} message PostSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                PostSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        object.postUrl = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.postUrl != null && message.hasOwnProperty("postUrl"))
                        object.postUrl = message.postUrl;
                    return object;
                };
    
                /**
                 * Converts this PostSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.PostSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                PostSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return PostSetting;
            })();
    
            selfomat.ListSetting = (function() {
    
                /**
                 * Properties of a ListSetting.
                 * @memberof xtech.selfomat
                 * @interface IListSetting
                 * @property {string} name ListSetting name
                 * @property {number} currentIndex ListSetting currentIndex
                 * @property {Array.<string>|null} [values] ListSetting values
                 * @property {string} updateUrl ListSetting updateUrl
                 */
    
                /**
                 * Constructs a new ListSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a ListSetting.
                 * @implements IListSetting
                 * @constructor
                 * @param {xtech.selfomat.IListSetting=} [properties] Properties to set
                 */
                function ListSetting(properties) {
                    this.values = [];
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * ListSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.ListSetting
                 * @instance
                 */
                ListSetting.prototype.name = "";
    
                /**
                 * ListSetting currentIndex.
                 * @member {number} currentIndex
                 * @memberof xtech.selfomat.ListSetting
                 * @instance
                 */
                ListSetting.prototype.currentIndex = 0;
    
                /**
                 * ListSetting values.
                 * @member {Array.<string>} values
                 * @memberof xtech.selfomat.ListSetting
                 * @instance
                 */
                ListSetting.prototype.values = $util.emptyArray;
    
                /**
                 * ListSetting updateUrl.
                 * @member {string} updateUrl
                 * @memberof xtech.selfomat.ListSetting
                 * @instance
                 */
                ListSetting.prototype.updateUrl = "";
    
                /**
                 * Creates a new ListSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {xtech.selfomat.IListSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.ListSetting} ListSetting instance
                 */
                ListSetting.create = function create(properties) {
                    return new ListSetting(properties);
                };
    
                /**
                 * Encodes the specified ListSetting message. Does not implicitly {@link xtech.selfomat.ListSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {xtech.selfomat.IListSetting} message ListSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                ListSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 0 =*/16).int32(message.currentIndex);
                    if (message.values != null && message.values.length)
                        for (var i = 0; i < message.values.length; ++i)
                            writer.uint32(/* id 3, wireType 2 =*/26).string(message.values[i]);
                    writer.uint32(/* id 4, wireType 2 =*/34).string(message.updateUrl);
                    return writer;
                };
    
                /**
                 * Encodes the specified ListSetting message, length delimited. Does not implicitly {@link xtech.selfomat.ListSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {xtech.selfomat.IListSetting} message ListSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                ListSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a ListSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.ListSetting} ListSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                ListSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.ListSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.currentIndex = reader.int32();
                            break;
                        case 3:
                            if (!(message.values && message.values.length))
                                message.values = [];
                            message.values.push(reader.string());
                            break;
                        case 4:
                            message.updateUrl = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("currentIndex"))
                        throw $util.ProtocolError("missing required 'currentIndex'", { instance: message });
                    if (!message.hasOwnProperty("updateUrl"))
                        throw $util.ProtocolError("missing required 'updateUrl'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a ListSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.ListSetting} ListSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                ListSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a ListSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                ListSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (!$util.isInteger(message.currentIndex))
                        return "currentIndex: integer expected";
                    if (message.values != null && message.hasOwnProperty("values")) {
                        if (!Array.isArray(message.values))
                            return "values: array expected";
                        for (var i = 0; i < message.values.length; ++i)
                            if (!$util.isString(message.values[i]))
                                return "values: string[] expected";
                    }
                    if (!$util.isString(message.updateUrl))
                        return "updateUrl: string expected";
                    return null;
                };
    
                /**
                 * Creates a ListSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.ListSetting} ListSetting
                 */
                ListSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.ListSetting)
                        return object;
                    var message = new $root.xtech.selfomat.ListSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.currentIndex != null)
                        message.currentIndex = object.currentIndex | 0;
                    if (object.values) {
                        if (!Array.isArray(object.values))
                            throw TypeError(".xtech.selfomat.ListSetting.values: array expected");
                        message.values = [];
                        for (var i = 0; i < object.values.length; ++i)
                            message.values[i] = String(object.values[i]);
                    }
                    if (object.updateUrl != null)
                        message.updateUrl = String(object.updateUrl);
                    return message;
                };
    
                /**
                 * Creates a plain object from a ListSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.ListSetting
                 * @static
                 * @param {xtech.selfomat.ListSetting} message ListSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                ListSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.arrays || options.defaults)
                        object.values = [];
                    if (options.defaults) {
                        object.name = "";
                        object.currentIndex = 0;
                        object.updateUrl = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.currentIndex != null && message.hasOwnProperty("currentIndex"))
                        object.currentIndex = message.currentIndex;
                    if (message.values && message.values.length) {
                        object.values = [];
                        for (var j = 0; j < message.values.length; ++j)
                            object.values[j] = message.values[j];
                    }
                    if (message.updateUrl != null && message.hasOwnProperty("updateUrl"))
                        object.updateUrl = message.updateUrl;
                    return object;
                };
    
                /**
                 * Converts this ListSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.ListSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                ListSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return ListSetting;
            })();
    
            selfomat.BoolSetting = (function() {
    
                /**
                 * Properties of a BoolSetting.
                 * @memberof xtech.selfomat
                 * @interface IBoolSetting
                 * @property {string} name BoolSetting name
                 * @property {boolean} currentValue BoolSetting currentValue
                 * @property {string} updateUrl BoolSetting updateUrl
                 */
    
                /**
                 * Constructs a new BoolSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a BoolSetting.
                 * @implements IBoolSetting
                 * @constructor
                 * @param {xtech.selfomat.IBoolSetting=} [properties] Properties to set
                 */
                function BoolSetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * BoolSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.BoolSetting
                 * @instance
                 */
                BoolSetting.prototype.name = "";
    
                /**
                 * BoolSetting currentValue.
                 * @member {boolean} currentValue
                 * @memberof xtech.selfomat.BoolSetting
                 * @instance
                 */
                BoolSetting.prototype.currentValue = false;
    
                /**
                 * BoolSetting updateUrl.
                 * @member {string} updateUrl
                 * @memberof xtech.selfomat.BoolSetting
                 * @instance
                 */
                BoolSetting.prototype.updateUrl = "";
    
                /**
                 * Creates a new BoolSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {xtech.selfomat.IBoolSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.BoolSetting} BoolSetting instance
                 */
                BoolSetting.create = function create(properties) {
                    return new BoolSetting(properties);
                };
    
                /**
                 * Encodes the specified BoolSetting message. Does not implicitly {@link xtech.selfomat.BoolSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {xtech.selfomat.IBoolSetting} message BoolSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoolSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 0 =*/16).bool(message.currentValue);
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.updateUrl);
                    return writer;
                };
    
                /**
                 * Encodes the specified BoolSetting message, length delimited. Does not implicitly {@link xtech.selfomat.BoolSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {xtech.selfomat.IBoolSetting} message BoolSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoolSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a BoolSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.BoolSetting} BoolSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoolSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.BoolSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.currentValue = reader.bool();
                            break;
                        case 3:
                            message.updateUrl = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("currentValue"))
                        throw $util.ProtocolError("missing required 'currentValue'", { instance: message });
                    if (!message.hasOwnProperty("updateUrl"))
                        throw $util.ProtocolError("missing required 'updateUrl'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a BoolSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.BoolSetting} BoolSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoolSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a BoolSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                BoolSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (typeof message.currentValue !== "boolean")
                        return "currentValue: boolean expected";
                    if (!$util.isString(message.updateUrl))
                        return "updateUrl: string expected";
                    return null;
                };
    
                /**
                 * Creates a BoolSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.BoolSetting} BoolSetting
                 */
                BoolSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.BoolSetting)
                        return object;
                    var message = new $root.xtech.selfomat.BoolSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.currentValue != null)
                        message.currentValue = Boolean(object.currentValue);
                    if (object.updateUrl != null)
                        message.updateUrl = String(object.updateUrl);
                    return message;
                };
    
                /**
                 * Creates a plain object from a BoolSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.BoolSetting
                 * @static
                 * @param {xtech.selfomat.BoolSetting} message BoolSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                BoolSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        object.currentValue = false;
                        object.updateUrl = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.currentValue != null && message.hasOwnProperty("currentValue"))
                        object.currentValue = message.currentValue;
                    if (message.updateUrl != null && message.hasOwnProperty("updateUrl"))
                        object.updateUrl = message.updateUrl;
                    return object;
                };
    
                /**
                 * Converts this BoolSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.BoolSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                BoolSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return BoolSetting;
            })();
    
            selfomat.FloatSetting = (function() {
    
                /**
                 * Properties of a FloatSetting.
                 * @memberof xtech.selfomat
                 * @interface IFloatSetting
                 * @property {string} name FloatSetting name
                 * @property {number} currentValue FloatSetting currentValue
                 * @property {number} minValue FloatSetting minValue
                 * @property {number} maxValue FloatSetting maxValue
                 * @property {string} updateUrl FloatSetting updateUrl
                 */
    
                /**
                 * Constructs a new FloatSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a FloatSetting.
                 * @implements IFloatSetting
                 * @constructor
                 * @param {xtech.selfomat.IFloatSetting=} [properties] Properties to set
                 */
                function FloatSetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * FloatSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.FloatSetting
                 * @instance
                 */
                FloatSetting.prototype.name = "";
    
                /**
                 * FloatSetting currentValue.
                 * @member {number} currentValue
                 * @memberof xtech.selfomat.FloatSetting
                 * @instance
                 */
                FloatSetting.prototype.currentValue = 0;
    
                /**
                 * FloatSetting minValue.
                 * @member {number} minValue
                 * @memberof xtech.selfomat.FloatSetting
                 * @instance
                 */
                FloatSetting.prototype.minValue = 0;
    
                /**
                 * FloatSetting maxValue.
                 * @member {number} maxValue
                 * @memberof xtech.selfomat.FloatSetting
                 * @instance
                 */
                FloatSetting.prototype.maxValue = 0;
    
                /**
                 * FloatSetting updateUrl.
                 * @member {string} updateUrl
                 * @memberof xtech.selfomat.FloatSetting
                 * @instance
                 */
                FloatSetting.prototype.updateUrl = "";
    
                /**
                 * Creates a new FloatSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {xtech.selfomat.IFloatSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.FloatSetting} FloatSetting instance
                 */
                FloatSetting.create = function create(properties) {
                    return new FloatSetting(properties);
                };
    
                /**
                 * Encodes the specified FloatSetting message. Does not implicitly {@link xtech.selfomat.FloatSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {xtech.selfomat.IFloatSetting} message FloatSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                FloatSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 5 =*/21).float(message.currentValue);
                    writer.uint32(/* id 3, wireType 5 =*/29).float(message.minValue);
                    writer.uint32(/* id 4, wireType 5 =*/37).float(message.maxValue);
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.updateUrl);
                    return writer;
                };
    
                /**
                 * Encodes the specified FloatSetting message, length delimited. Does not implicitly {@link xtech.selfomat.FloatSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {xtech.selfomat.IFloatSetting} message FloatSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                FloatSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a FloatSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.FloatSetting} FloatSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                FloatSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.FloatSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.currentValue = reader.float();
                            break;
                        case 3:
                            message.minValue = reader.float();
                            break;
                        case 4:
                            message.maxValue = reader.float();
                            break;
                        case 5:
                            message.updateUrl = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("currentValue"))
                        throw $util.ProtocolError("missing required 'currentValue'", { instance: message });
                    if (!message.hasOwnProperty("minValue"))
                        throw $util.ProtocolError("missing required 'minValue'", { instance: message });
                    if (!message.hasOwnProperty("maxValue"))
                        throw $util.ProtocolError("missing required 'maxValue'", { instance: message });
                    if (!message.hasOwnProperty("updateUrl"))
                        throw $util.ProtocolError("missing required 'updateUrl'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a FloatSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.FloatSetting} FloatSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                FloatSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a FloatSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                FloatSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (typeof message.currentValue !== "number")
                        return "currentValue: number expected";
                    if (typeof message.minValue !== "number")
                        return "minValue: number expected";
                    if (typeof message.maxValue !== "number")
                        return "maxValue: number expected";
                    if (!$util.isString(message.updateUrl))
                        return "updateUrl: string expected";
                    return null;
                };
    
                /**
                 * Creates a FloatSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.FloatSetting} FloatSetting
                 */
                FloatSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.FloatSetting)
                        return object;
                    var message = new $root.xtech.selfomat.FloatSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.currentValue != null)
                        message.currentValue = Number(object.currentValue);
                    if (object.minValue != null)
                        message.minValue = Number(object.minValue);
                    if (object.maxValue != null)
                        message.maxValue = Number(object.maxValue);
                    if (object.updateUrl != null)
                        message.updateUrl = String(object.updateUrl);
                    return message;
                };
    
                /**
                 * Creates a plain object from a FloatSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.FloatSetting
                 * @static
                 * @param {xtech.selfomat.FloatSetting} message FloatSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                FloatSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        object.currentValue = 0;
                        object.minValue = 0;
                        object.maxValue = 0;
                        object.updateUrl = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.currentValue != null && message.hasOwnProperty("currentValue"))
                        object.currentValue = options.json && !isFinite(message.currentValue) ? String(message.currentValue) : message.currentValue;
                    if (message.minValue != null && message.hasOwnProperty("minValue"))
                        object.minValue = options.json && !isFinite(message.minValue) ? String(message.minValue) : message.minValue;
                    if (message.maxValue != null && message.hasOwnProperty("maxValue"))
                        object.maxValue = options.json && !isFinite(message.maxValue) ? String(message.maxValue) : message.maxValue;
                    if (message.updateUrl != null && message.hasOwnProperty("updateUrl"))
                        object.updateUrl = message.updateUrl;
                    return object;
                };
    
                /**
                 * Converts this FloatSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.FloatSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                FloatSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return FloatSetting;
            })();
    
            selfomat.IntSetting = (function() {
    
                /**
                 * Properties of an IntSetting.
                 * @memberof xtech.selfomat
                 * @interface IIntSetting
                 * @property {string} name IntSetting name
                 * @property {number|Long} currentValue IntSetting currentValue
                 * @property {number|Long} minValue IntSetting minValue
                 * @property {number|Long} maxValue IntSetting maxValue
                 * @property {string} updateUrl IntSetting updateUrl
                 */
    
                /**
                 * Constructs a new IntSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents an IntSetting.
                 * @implements IIntSetting
                 * @constructor
                 * @param {xtech.selfomat.IIntSetting=} [properties] Properties to set
                 */
                function IntSetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * IntSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.IntSetting
                 * @instance
                 */
                IntSetting.prototype.name = "";
    
                /**
                 * IntSetting currentValue.
                 * @member {number|Long} currentValue
                 * @memberof xtech.selfomat.IntSetting
                 * @instance
                 */
                IntSetting.prototype.currentValue = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
                /**
                 * IntSetting minValue.
                 * @member {number|Long} minValue
                 * @memberof xtech.selfomat.IntSetting
                 * @instance
                 */
                IntSetting.prototype.minValue = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
                /**
                 * IntSetting maxValue.
                 * @member {number|Long} maxValue
                 * @memberof xtech.selfomat.IntSetting
                 * @instance
                 */
                IntSetting.prototype.maxValue = $util.Long ? $util.Long.fromBits(0,0,false) : 0;
    
                /**
                 * IntSetting updateUrl.
                 * @member {string} updateUrl
                 * @memberof xtech.selfomat.IntSetting
                 * @instance
                 */
                IntSetting.prototype.updateUrl = "";
    
                /**
                 * Creates a new IntSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {xtech.selfomat.IIntSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.IntSetting} IntSetting instance
                 */
                IntSetting.create = function create(properties) {
                    return new IntSetting(properties);
                };
    
                /**
                 * Encodes the specified IntSetting message. Does not implicitly {@link xtech.selfomat.IntSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {xtech.selfomat.IIntSetting} message IntSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                IntSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 0 =*/16).int64(message.currentValue);
                    writer.uint32(/* id 3, wireType 0 =*/24).int64(message.minValue);
                    writer.uint32(/* id 4, wireType 0 =*/32).int64(message.maxValue);
                    writer.uint32(/* id 5, wireType 2 =*/42).string(message.updateUrl);
                    return writer;
                };
    
                /**
                 * Encodes the specified IntSetting message, length delimited. Does not implicitly {@link xtech.selfomat.IntSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {xtech.selfomat.IIntSetting} message IntSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                IntSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes an IntSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.IntSetting} IntSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                IntSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.IntSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.currentValue = reader.int64();
                            break;
                        case 3:
                            message.minValue = reader.int64();
                            break;
                        case 4:
                            message.maxValue = reader.int64();
                            break;
                        case 5:
                            message.updateUrl = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("currentValue"))
                        throw $util.ProtocolError("missing required 'currentValue'", { instance: message });
                    if (!message.hasOwnProperty("minValue"))
                        throw $util.ProtocolError("missing required 'minValue'", { instance: message });
                    if (!message.hasOwnProperty("maxValue"))
                        throw $util.ProtocolError("missing required 'maxValue'", { instance: message });
                    if (!message.hasOwnProperty("updateUrl"))
                        throw $util.ProtocolError("missing required 'updateUrl'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes an IntSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.IntSetting} IntSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                IntSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies an IntSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                IntSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (!$util.isInteger(message.currentValue) && !(message.currentValue && $util.isInteger(message.currentValue.low) && $util.isInteger(message.currentValue.high)))
                        return "currentValue: integer|Long expected";
                    if (!$util.isInteger(message.minValue) && !(message.minValue && $util.isInteger(message.minValue.low) && $util.isInteger(message.minValue.high)))
                        return "minValue: integer|Long expected";
                    if (!$util.isInteger(message.maxValue) && !(message.maxValue && $util.isInteger(message.maxValue.low) && $util.isInteger(message.maxValue.high)))
                        return "maxValue: integer|Long expected";
                    if (!$util.isString(message.updateUrl))
                        return "updateUrl: string expected";
                    return null;
                };
    
                /**
                 * Creates an IntSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.IntSetting} IntSetting
                 */
                IntSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.IntSetting)
                        return object;
                    var message = new $root.xtech.selfomat.IntSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.currentValue != null)
                        if ($util.Long)
                            (message.currentValue = $util.Long.fromValue(object.currentValue)).unsigned = false;
                        else if (typeof object.currentValue === "string")
                            message.currentValue = parseInt(object.currentValue, 10);
                        else if (typeof object.currentValue === "number")
                            message.currentValue = object.currentValue;
                        else if (typeof object.currentValue === "object")
                            message.currentValue = new $util.LongBits(object.currentValue.low >>> 0, object.currentValue.high >>> 0).toNumber();
                    if (object.minValue != null)
                        if ($util.Long)
                            (message.minValue = $util.Long.fromValue(object.minValue)).unsigned = false;
                        else if (typeof object.minValue === "string")
                            message.minValue = parseInt(object.minValue, 10);
                        else if (typeof object.minValue === "number")
                            message.minValue = object.minValue;
                        else if (typeof object.minValue === "object")
                            message.minValue = new $util.LongBits(object.minValue.low >>> 0, object.minValue.high >>> 0).toNumber();
                    if (object.maxValue != null)
                        if ($util.Long)
                            (message.maxValue = $util.Long.fromValue(object.maxValue)).unsigned = false;
                        else if (typeof object.maxValue === "string")
                            message.maxValue = parseInt(object.maxValue, 10);
                        else if (typeof object.maxValue === "number")
                            message.maxValue = object.maxValue;
                        else if (typeof object.maxValue === "object")
                            message.maxValue = new $util.LongBits(object.maxValue.low >>> 0, object.maxValue.high >>> 0).toNumber();
                    if (object.updateUrl != null)
                        message.updateUrl = String(object.updateUrl);
                    return message;
                };
    
                /**
                 * Creates a plain object from an IntSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.IntSetting
                 * @static
                 * @param {xtech.selfomat.IntSetting} message IntSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                IntSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        if ($util.Long) {
                            var long = new $util.Long(0, 0, false);
                            object.currentValue = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                        } else
                            object.currentValue = options.longs === String ? "0" : 0;
                        if ($util.Long) {
                            var long = new $util.Long(0, 0, false);
                            object.minValue = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                        } else
                            object.minValue = options.longs === String ? "0" : 0;
                        if ($util.Long) {
                            var long = new $util.Long(0, 0, false);
                            object.maxValue = options.longs === String ? long.toString() : options.longs === Number ? long.toNumber() : long;
                        } else
                            object.maxValue = options.longs === String ? "0" : 0;
                        object.updateUrl = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.currentValue != null && message.hasOwnProperty("currentValue"))
                        if (typeof message.currentValue === "number")
                            object.currentValue = options.longs === String ? String(message.currentValue) : message.currentValue;
                        else
                            object.currentValue = options.longs === String ? $util.Long.prototype.toString.call(message.currentValue) : options.longs === Number ? new $util.LongBits(message.currentValue.low >>> 0, message.currentValue.high >>> 0).toNumber() : message.currentValue;
                    if (message.minValue != null && message.hasOwnProperty("minValue"))
                        if (typeof message.minValue === "number")
                            object.minValue = options.longs === String ? String(message.minValue) : message.minValue;
                        else
                            object.minValue = options.longs === String ? $util.Long.prototype.toString.call(message.minValue) : options.longs === Number ? new $util.LongBits(message.minValue.low >>> 0, message.minValue.high >>> 0).toNumber() : message.minValue;
                    if (message.maxValue != null && message.hasOwnProperty("maxValue"))
                        if (typeof message.maxValue === "number")
                            object.maxValue = options.longs === String ? String(message.maxValue) : message.maxValue;
                        else
                            object.maxValue = options.longs === String ? $util.Long.prototype.toString.call(message.maxValue) : options.longs === Number ? new $util.LongBits(message.maxValue.low >>> 0, message.maxValue.high >>> 0).toNumber() : message.maxValue;
                    if (message.updateUrl != null && message.hasOwnProperty("updateUrl"))
                        object.updateUrl = message.updateUrl;
                    return object;
                };
    
                /**
                 * Converts this IntSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.IntSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                IntSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return IntSetting;
            })();
    
            selfomat.CameraSettings = (function() {
    
                /**
                 * Properties of a CameraSettings.
                 * @memberof xtech.selfomat
                 * @interface ICameraSettings
                 * @property {xtech.selfomat.IListSetting} iso CameraSettings iso
                 * @property {xtech.selfomat.IListSetting} shutterSpeed CameraSettings shutterSpeed
                 * @property {xtech.selfomat.IListSetting} aperture CameraSettings aperture
                 * @property {xtech.selfomat.IListSetting} exposureCompensation CameraSettings exposureCompensation
                 * @property {xtech.selfomat.IListSetting} imageFormat CameraSettings imageFormat
                 * @property {xtech.selfomat.IReadOnlySetting} cameraName CameraSettings cameraName
                 * @property {xtech.selfomat.IReadOnlySetting} lensName CameraSettings lensName
                 * @property {xtech.selfomat.IPostSetting|null} [focus] CameraSettings focus
                 */
    
                /**
                 * Constructs a new CameraSettings.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a CameraSettings.
                 * @implements ICameraSettings
                 * @constructor
                 * @param {xtech.selfomat.ICameraSettings=} [properties] Properties to set
                 */
                function CameraSettings(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * CameraSettings iso.
                 * @member {xtech.selfomat.IListSetting} iso
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.iso = null;
    
                /**
                 * CameraSettings shutterSpeed.
                 * @member {xtech.selfomat.IListSetting} shutterSpeed
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.shutterSpeed = null;
    
                /**
                 * CameraSettings aperture.
                 * @member {xtech.selfomat.IListSetting} aperture
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.aperture = null;
    
                /**
                 * CameraSettings exposureCompensation.
                 * @member {xtech.selfomat.IListSetting} exposureCompensation
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.exposureCompensation = null;
    
                /**
                 * CameraSettings imageFormat.
                 * @member {xtech.selfomat.IListSetting} imageFormat
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.imageFormat = null;
    
                /**
                 * CameraSettings cameraName.
                 * @member {xtech.selfomat.IReadOnlySetting} cameraName
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.cameraName = null;
    
                /**
                 * CameraSettings lensName.
                 * @member {xtech.selfomat.IReadOnlySetting} lensName
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.lensName = null;
    
                /**
                 * CameraSettings focus.
                 * @member {xtech.selfomat.IPostSetting|null|undefined} focus
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.focus = null;
    
                /**
                 * Creates a new CameraSettings instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {xtech.selfomat.ICameraSettings=} [properties] Properties to set
                 * @returns {xtech.selfomat.CameraSettings} CameraSettings instance
                 */
                CameraSettings.create = function create(properties) {
                    return new CameraSettings(properties);
                };
    
                /**
                 * Encodes the specified CameraSettings message. Does not implicitly {@link xtech.selfomat.CameraSettings.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {xtech.selfomat.ICameraSettings} message CameraSettings message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                CameraSettings.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    $root.xtech.selfomat.ListSetting.encode(message.iso, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.shutterSpeed, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.aperture, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.exposureCompensation, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.imageFormat, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                    $root.xtech.selfomat.ReadOnlySetting.encode(message.cameraName, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                    $root.xtech.selfomat.ReadOnlySetting.encode(message.lensName, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                    if (message.focus != null && message.hasOwnProperty("focus"))
                        $root.xtech.selfomat.PostSetting.encode(message.focus, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                    return writer;
                };
    
                /**
                 * Encodes the specified CameraSettings message, length delimited. Does not implicitly {@link xtech.selfomat.CameraSettings.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {xtech.selfomat.ICameraSettings} message CameraSettings message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                CameraSettings.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a CameraSettings message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.CameraSettings} CameraSettings
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                CameraSettings.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.CameraSettings();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.iso = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 2:
                            message.shutterSpeed = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 3:
                            message.aperture = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 4:
                            message.exposureCompensation = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 5:
                            message.imageFormat = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 7:
                            message.cameraName = $root.xtech.selfomat.ReadOnlySetting.decode(reader, reader.uint32());
                            break;
                        case 8:
                            message.lensName = $root.xtech.selfomat.ReadOnlySetting.decode(reader, reader.uint32());
                            break;
                        case 9:
                            message.focus = $root.xtech.selfomat.PostSetting.decode(reader, reader.uint32());
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("iso"))
                        throw $util.ProtocolError("missing required 'iso'", { instance: message });
                    if (!message.hasOwnProperty("shutterSpeed"))
                        throw $util.ProtocolError("missing required 'shutterSpeed'", { instance: message });
                    if (!message.hasOwnProperty("aperture"))
                        throw $util.ProtocolError("missing required 'aperture'", { instance: message });
                    if (!message.hasOwnProperty("exposureCompensation"))
                        throw $util.ProtocolError("missing required 'exposureCompensation'", { instance: message });
                    if (!message.hasOwnProperty("imageFormat"))
                        throw $util.ProtocolError("missing required 'imageFormat'", { instance: message });
                    if (!message.hasOwnProperty("cameraName"))
                        throw $util.ProtocolError("missing required 'cameraName'", { instance: message });
                    if (!message.hasOwnProperty("lensName"))
                        throw $util.ProtocolError("missing required 'lensName'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a CameraSettings message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.CameraSettings} CameraSettings
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                CameraSettings.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a CameraSettings message.
                 * @function verify
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                CameraSettings.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.iso);
                        if (error)
                            return "iso." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.shutterSpeed);
                        if (error)
                            return "shutterSpeed." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.aperture);
                        if (error)
                            return "aperture." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.exposureCompensation);
                        if (error)
                            return "exposureCompensation." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.imageFormat);
                        if (error)
                            return "imageFormat." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ReadOnlySetting.verify(message.cameraName);
                        if (error)
                            return "cameraName." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ReadOnlySetting.verify(message.lensName);
                        if (error)
                            return "lensName." + error;
                    }
                    if (message.focus != null && message.hasOwnProperty("focus")) {
                        var error = $root.xtech.selfomat.PostSetting.verify(message.focus);
                        if (error)
                            return "focus." + error;
                    }
                    return null;
                };
    
                /**
                 * Creates a CameraSettings message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.CameraSettings} CameraSettings
                 */
                CameraSettings.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.CameraSettings)
                        return object;
                    var message = new $root.xtech.selfomat.CameraSettings();
                    if (object.iso != null) {
                        if (typeof object.iso !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.iso: object expected");
                        message.iso = $root.xtech.selfomat.ListSetting.fromObject(object.iso);
                    }
                    if (object.shutterSpeed != null) {
                        if (typeof object.shutterSpeed !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.shutterSpeed: object expected");
                        message.shutterSpeed = $root.xtech.selfomat.ListSetting.fromObject(object.shutterSpeed);
                    }
                    if (object.aperture != null) {
                        if (typeof object.aperture !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.aperture: object expected");
                        message.aperture = $root.xtech.selfomat.ListSetting.fromObject(object.aperture);
                    }
                    if (object.exposureCompensation != null) {
                        if (typeof object.exposureCompensation !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.exposureCompensation: object expected");
                        message.exposureCompensation = $root.xtech.selfomat.ListSetting.fromObject(object.exposureCompensation);
                    }
                    if (object.imageFormat != null) {
                        if (typeof object.imageFormat !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.imageFormat: object expected");
                        message.imageFormat = $root.xtech.selfomat.ListSetting.fromObject(object.imageFormat);
                    }
                    if (object.cameraName != null) {
                        if (typeof object.cameraName !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.cameraName: object expected");
                        message.cameraName = $root.xtech.selfomat.ReadOnlySetting.fromObject(object.cameraName);
                    }
                    if (object.lensName != null) {
                        if (typeof object.lensName !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.lensName: object expected");
                        message.lensName = $root.xtech.selfomat.ReadOnlySetting.fromObject(object.lensName);
                    }
                    if (object.focus != null) {
                        if (typeof object.focus !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.focus: object expected");
                        message.focus = $root.xtech.selfomat.PostSetting.fromObject(object.focus);
                    }
                    return message;
                };
    
                /**
                 * Creates a plain object from a CameraSettings message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.CameraSettings
                 * @static
                 * @param {xtech.selfomat.CameraSettings} message CameraSettings
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                CameraSettings.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.iso = null;
                        object.shutterSpeed = null;
                        object.aperture = null;
                        object.exposureCompensation = null;
                        object.imageFormat = null;
                        object.cameraName = null;
                        object.lensName = null;
                        object.focus = null;
                    }
                    if (message.iso != null && message.hasOwnProperty("iso"))
                        object.iso = $root.xtech.selfomat.ListSetting.toObject(message.iso, options);
                    if (message.shutterSpeed != null && message.hasOwnProperty("shutterSpeed"))
                        object.shutterSpeed = $root.xtech.selfomat.ListSetting.toObject(message.shutterSpeed, options);
                    if (message.aperture != null && message.hasOwnProperty("aperture"))
                        object.aperture = $root.xtech.selfomat.ListSetting.toObject(message.aperture, options);
                    if (message.exposureCompensation != null && message.hasOwnProperty("exposureCompensation"))
                        object.exposureCompensation = $root.xtech.selfomat.ListSetting.toObject(message.exposureCompensation, options);
                    if (message.imageFormat != null && message.hasOwnProperty("imageFormat"))
                        object.imageFormat = $root.xtech.selfomat.ListSetting.toObject(message.imageFormat, options);
                    if (message.cameraName != null && message.hasOwnProperty("cameraName"))
                        object.cameraName = $root.xtech.selfomat.ReadOnlySetting.toObject(message.cameraName, options);
                    if (message.lensName != null && message.hasOwnProperty("lensName"))
                        object.lensName = $root.xtech.selfomat.ReadOnlySetting.toObject(message.lensName, options);
                    if (message.focus != null && message.hasOwnProperty("focus"))
                        object.focus = $root.xtech.selfomat.PostSetting.toObject(message.focus, options);
                    return object;
                };
    
                /**
                 * Converts this CameraSettings to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                CameraSettings.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return CameraSettings;
            })();
    
            selfomat.BoothSettings = (function() {
    
                /**
                 * Properties of a BoothSettings.
                 * @memberof xtech.selfomat
                 * @interface IBoothSettings
                 * @property {xtech.selfomat.IBoolSetting} printerEnabled BoothSettings printerEnabled
                 * @property {xtech.selfomat.IBoolSetting} flashEnabled BoothSettings flashEnabled
                 * @property {xtech.selfomat.IFloatSetting} flashBrightness BoothSettings flashBrightness
                 * @property {xtech.selfomat.IFloatSetting} flashFade BoothSettings flashFade
                 * @property {xtech.selfomat.IIntSetting} flashDelayMicros BoothSettings flashDelayMicros
                 * @property {xtech.selfomat.IIntSetting} flashDurationMicros BoothSettings flashDurationMicros
                 * @property {xtech.selfomat.IListSetting} flashExposureCompensation BoothSettings flashExposureCompensation
                 * @property {xtech.selfomat.IBoolSetting} templateEnabled BoothSettings templateEnabled
                 * @property {xtech.selfomat.IListSetting} ledOffset BoothSettings ledOffset
                 */
    
                /**
                 * Constructs a new BoothSettings.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a BoothSettings.
                 * @implements IBoothSettings
                 * @constructor
                 * @param {xtech.selfomat.IBoothSettings=} [properties] Properties to set
                 */
                function BoothSettings(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * BoothSettings printerEnabled.
                 * @member {xtech.selfomat.IBoolSetting} printerEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.printerEnabled = null;
    
                /**
                 * BoothSettings flashEnabled.
                 * @member {xtech.selfomat.IBoolSetting} flashEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashEnabled = null;
    
                /**
                 * BoothSettings flashBrightness.
                 * @member {xtech.selfomat.IFloatSetting} flashBrightness
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashBrightness = null;
    
                /**
                 * BoothSettings flashFade.
                 * @member {xtech.selfomat.IFloatSetting} flashFade
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashFade = null;
    
                /**
                 * BoothSettings flashDelayMicros.
                 * @member {xtech.selfomat.IIntSetting} flashDelayMicros
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashDelayMicros = null;
    
                /**
                 * BoothSettings flashDurationMicros.
                 * @member {xtech.selfomat.IIntSetting} flashDurationMicros
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashDurationMicros = null;
    
                /**
                 * BoothSettings flashExposureCompensation.
                 * @member {xtech.selfomat.IListSetting} flashExposureCompensation
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashExposureCompensation = null;
    
                /**
                 * BoothSettings templateEnabled.
                 * @member {xtech.selfomat.IBoolSetting} templateEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.templateEnabled = null;
    
                /**
                 * BoothSettings ledOffset.
                 * @member {xtech.selfomat.IListSetting} ledOffset
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.ledOffset = null;
    
                /**
                 * Creates a new BoothSettings instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {xtech.selfomat.IBoothSettings=} [properties] Properties to set
                 * @returns {xtech.selfomat.BoothSettings} BoothSettings instance
                 */
                BoothSettings.create = function create(properties) {
                    return new BoothSettings(properties);
                };
    
                /**
                 * Encodes the specified BoothSettings message. Does not implicitly {@link xtech.selfomat.BoothSettings.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {xtech.selfomat.IBoothSettings} message BoothSettings message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoothSettings.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    $root.xtech.selfomat.BoolSetting.encode(message.printerEnabled, writer.uint32(/* id 1, wireType 2 =*/10).fork()).ldelim();
                    $root.xtech.selfomat.BoolSetting.encode(message.flashEnabled, writer.uint32(/* id 2, wireType 2 =*/18).fork()).ldelim();
                    $root.xtech.selfomat.FloatSetting.encode(message.flashBrightness, writer.uint32(/* id 3, wireType 2 =*/26).fork()).ldelim();
                    $root.xtech.selfomat.FloatSetting.encode(message.flashFade, writer.uint32(/* id 4, wireType 2 =*/34).fork()).ldelim();
                    $root.xtech.selfomat.IntSetting.encode(message.flashDelayMicros, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                    $root.xtech.selfomat.IntSetting.encode(message.flashDurationMicros, writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.flashExposureCompensation, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                    $root.xtech.selfomat.BoolSetting.encode(message.templateEnabled, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.ledOffset, writer.uint32(/* id 9, wireType 2 =*/74).fork()).ldelim();
                    return writer;
                };
    
                /**
                 * Encodes the specified BoothSettings message, length delimited. Does not implicitly {@link xtech.selfomat.BoothSettings.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {xtech.selfomat.IBoothSettings} message BoothSettings message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoothSettings.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a BoothSettings message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.BoothSettings} BoothSettings
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoothSettings.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.BoothSettings();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.printerEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 2:
                            message.flashEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 3:
                            message.flashBrightness = $root.xtech.selfomat.FloatSetting.decode(reader, reader.uint32());
                            break;
                        case 4:
                            message.flashFade = $root.xtech.selfomat.FloatSetting.decode(reader, reader.uint32());
                            break;
                        case 5:
                            message.flashDelayMicros = $root.xtech.selfomat.IntSetting.decode(reader, reader.uint32());
                            break;
                        case 6:
                            message.flashDurationMicros = $root.xtech.selfomat.IntSetting.decode(reader, reader.uint32());
                            break;
                        case 7:
                            message.flashExposureCompensation = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 8:
                            message.templateEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 9:
                            message.ledOffset = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("printerEnabled"))
                        throw $util.ProtocolError("missing required 'printerEnabled'", { instance: message });
                    if (!message.hasOwnProperty("flashEnabled"))
                        throw $util.ProtocolError("missing required 'flashEnabled'", { instance: message });
                    if (!message.hasOwnProperty("flashBrightness"))
                        throw $util.ProtocolError("missing required 'flashBrightness'", { instance: message });
                    if (!message.hasOwnProperty("flashFade"))
                        throw $util.ProtocolError("missing required 'flashFade'", { instance: message });
                    if (!message.hasOwnProperty("flashDelayMicros"))
                        throw $util.ProtocolError("missing required 'flashDelayMicros'", { instance: message });
                    if (!message.hasOwnProperty("flashDurationMicros"))
                        throw $util.ProtocolError("missing required 'flashDurationMicros'", { instance: message });
                    if (!message.hasOwnProperty("flashExposureCompensation"))
                        throw $util.ProtocolError("missing required 'flashExposureCompensation'", { instance: message });
                    if (!message.hasOwnProperty("templateEnabled"))
                        throw $util.ProtocolError("missing required 'templateEnabled'", { instance: message });
                    if (!message.hasOwnProperty("ledOffset"))
                        throw $util.ProtocolError("missing required 'ledOffset'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a BoothSettings message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.BoothSettings} BoothSettings
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoothSettings.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a BoothSettings message.
                 * @function verify
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                BoothSettings.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.printerEnabled);
                        if (error)
                            return "printerEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.flashEnabled);
                        if (error)
                            return "flashEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.FloatSetting.verify(message.flashBrightness);
                        if (error)
                            return "flashBrightness." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.FloatSetting.verify(message.flashFade);
                        if (error)
                            return "flashFade." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.IntSetting.verify(message.flashDelayMicros);
                        if (error)
                            return "flashDelayMicros." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.IntSetting.verify(message.flashDurationMicros);
                        if (error)
                            return "flashDurationMicros." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.flashExposureCompensation);
                        if (error)
                            return "flashExposureCompensation." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.templateEnabled);
                        if (error)
                            return "templateEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.ledOffset);
                        if (error)
                            return "ledOffset." + error;
                    }
                    return null;
                };
    
                /**
                 * Creates a BoothSettings message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.BoothSettings} BoothSettings
                 */
                BoothSettings.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.BoothSettings)
                        return object;
                    var message = new $root.xtech.selfomat.BoothSettings();
                    if (object.printerEnabled != null) {
                        if (typeof object.printerEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.printerEnabled: object expected");
                        message.printerEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.printerEnabled);
                    }
                    if (object.flashEnabled != null) {
                        if (typeof object.flashEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashEnabled: object expected");
                        message.flashEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.flashEnabled);
                    }
                    if (object.flashBrightness != null) {
                        if (typeof object.flashBrightness !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashBrightness: object expected");
                        message.flashBrightness = $root.xtech.selfomat.FloatSetting.fromObject(object.flashBrightness);
                    }
                    if (object.flashFade != null) {
                        if (typeof object.flashFade !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashFade: object expected");
                        message.flashFade = $root.xtech.selfomat.FloatSetting.fromObject(object.flashFade);
                    }
                    if (object.flashDelayMicros != null) {
                        if (typeof object.flashDelayMicros !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashDelayMicros: object expected");
                        message.flashDelayMicros = $root.xtech.selfomat.IntSetting.fromObject(object.flashDelayMicros);
                    }
                    if (object.flashDurationMicros != null) {
                        if (typeof object.flashDurationMicros !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashDurationMicros: object expected");
                        message.flashDurationMicros = $root.xtech.selfomat.IntSetting.fromObject(object.flashDurationMicros);
                    }
                    if (object.flashExposureCompensation != null) {
                        if (typeof object.flashExposureCompensation !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashExposureCompensation: object expected");
                        message.flashExposureCompensation = $root.xtech.selfomat.ListSetting.fromObject(object.flashExposureCompensation);
                    }
                    if (object.templateEnabled != null) {
                        if (typeof object.templateEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.templateEnabled: object expected");
                        message.templateEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.templateEnabled);
                    }
                    if (object.ledOffset != null) {
                        if (typeof object.ledOffset !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.ledOffset: object expected");
                        message.ledOffset = $root.xtech.selfomat.ListSetting.fromObject(object.ledOffset);
                    }
                    return message;
                };
    
                /**
                 * Creates a plain object from a BoothSettings message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.BoothSettings
                 * @static
                 * @param {xtech.selfomat.BoothSettings} message BoothSettings
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                BoothSettings.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.printerEnabled = null;
                        object.flashEnabled = null;
                        object.flashBrightness = null;
                        object.flashFade = null;
                        object.flashDelayMicros = null;
                        object.flashDurationMicros = null;
                        object.flashExposureCompensation = null;
                        object.templateEnabled = null;
                        object.ledOffset = null;
                    }
                    if (message.printerEnabled != null && message.hasOwnProperty("printerEnabled"))
                        object.printerEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.printerEnabled, options);
                    if (message.flashEnabled != null && message.hasOwnProperty("flashEnabled"))
                        object.flashEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.flashEnabled, options);
                    if (message.flashBrightness != null && message.hasOwnProperty("flashBrightness"))
                        object.flashBrightness = $root.xtech.selfomat.FloatSetting.toObject(message.flashBrightness, options);
                    if (message.flashFade != null && message.hasOwnProperty("flashFade"))
                        object.flashFade = $root.xtech.selfomat.FloatSetting.toObject(message.flashFade, options);
                    if (message.flashDelayMicros != null && message.hasOwnProperty("flashDelayMicros"))
                        object.flashDelayMicros = $root.xtech.selfomat.IntSetting.toObject(message.flashDelayMicros, options);
                    if (message.flashDurationMicros != null && message.hasOwnProperty("flashDurationMicros"))
                        object.flashDurationMicros = $root.xtech.selfomat.IntSetting.toObject(message.flashDurationMicros, options);
                    if (message.flashExposureCompensation != null && message.hasOwnProperty("flashExposureCompensation"))
                        object.flashExposureCompensation = $root.xtech.selfomat.ListSetting.toObject(message.flashExposureCompensation, options);
                    if (message.templateEnabled != null && message.hasOwnProperty("templateEnabled"))
                        object.templateEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.templateEnabled, options);
                    if (message.ledOffset != null && message.hasOwnProperty("ledOffset"))
                        object.ledOffset = $root.xtech.selfomat.ListSetting.toObject(message.ledOffset, options);
                    return object;
                };
    
                /**
                 * Converts this BoothSettings to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                BoothSettings.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return BoothSettings;
            })();
    
            return selfomat;
        })();
    
        return xtech;
    })();

    return $root;
});
