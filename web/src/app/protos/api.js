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
                 * @enum {number}
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
    
            selfomat.BoothError = (function() {
    
                /**
                 * Properties of a BoothError.
                 * @memberof xtech.selfomat
                 * @interface IBoothError
                 * @property {number} code BoothError code
                 * @property {string} title BoothError title
                 * @property {string} message BoothError message
                 */
    
                /**
                 * Constructs a new BoothError.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a BoothError.
                 * @implements IBoothError
                 * @constructor
                 * @param {xtech.selfomat.IBoothError=} [properties] Properties to set
                 */
                function BoothError(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * BoothError code.
                 * @member {number} code
                 * @memberof xtech.selfomat.BoothError
                 * @instance
                 */
                BoothError.prototype.code = 0;
    
                /**
                 * BoothError title.
                 * @member {string} title
                 * @memberof xtech.selfomat.BoothError
                 * @instance
                 */
                BoothError.prototype.title = "";
    
                /**
                 * BoothError message.
                 * @member {string} message
                 * @memberof xtech.selfomat.BoothError
                 * @instance
                 */
                BoothError.prototype.message = "";
    
                /**
                 * Creates a new BoothError instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {xtech.selfomat.IBoothError=} [properties] Properties to set
                 * @returns {xtech.selfomat.BoothError} BoothError instance
                 */
                BoothError.create = function create(properties) {
                    return new BoothError(properties);
                };
    
                /**
                 * Encodes the specified BoothError message. Does not implicitly {@link xtech.selfomat.BoothError.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {xtech.selfomat.IBoothError} message BoothError message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoothError.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 0 =*/8).int32(message.code);
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.title);
                    writer.uint32(/* id 3, wireType 2 =*/26).string(message.message);
                    return writer;
                };
    
                /**
                 * Encodes the specified BoothError message, length delimited. Does not implicitly {@link xtech.selfomat.BoothError.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {xtech.selfomat.IBoothError} message BoothError message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                BoothError.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a BoothError message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.BoothError} BoothError
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoothError.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.BoothError();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.code = reader.int32();
                            break;
                        case 2:
                            message.title = reader.string();
                            break;
                        case 3:
                            message.message = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("code"))
                        throw $util.ProtocolError("missing required 'code'", { instance: message });
                    if (!message.hasOwnProperty("title"))
                        throw $util.ProtocolError("missing required 'title'", { instance: message });
                    if (!message.hasOwnProperty("message"))
                        throw $util.ProtocolError("missing required 'message'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a BoothError message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.BoothError} BoothError
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                BoothError.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a BoothError message.
                 * @function verify
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                BoothError.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isInteger(message.code))
                        return "code: integer expected";
                    if (!$util.isString(message.title))
                        return "title: string expected";
                    if (!$util.isString(message.message))
                        return "message: string expected";
                    return null;
                };
    
                /**
                 * Creates a BoothError message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.BoothError} BoothError
                 */
                BoothError.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.BoothError)
                        return object;
                    var message = new $root.xtech.selfomat.BoothError();
                    if (object.code != null)
                        message.code = object.code | 0;
                    if (object.title != null)
                        message.title = String(object.title);
                    if (object.message != null)
                        message.message = String(object.message);
                    return message;
                };
    
                /**
                 * Creates a plain object from a BoothError message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.BoothError
                 * @static
                 * @param {xtech.selfomat.BoothError} message BoothError
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                BoothError.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.code = 0;
                        object.title = "";
                        object.message = "";
                    }
                    if (message.code != null && message.hasOwnProperty("code"))
                        object.code = message.code;
                    if (message.title != null && message.hasOwnProperty("title"))
                        object.title = message.title;
                    if (message.message != null && message.hasOwnProperty("message"))
                        object.message = message.message;
                    return object;
                };
    
                /**
                 * Converts this BoothError to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.BoothError
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                BoothError.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return BoothError;
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
                 * @property {string|null} [alert] PostSetting alert
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
                 * PostSetting alert.
                 * @member {string} alert
                 * @memberof xtech.selfomat.PostSetting
                 * @instance
                 */
                PostSetting.prototype.alert = "";
    
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
                    if (message.alert != null && Object.hasOwnProperty.call(message, "alert"))
                        writer.uint32(/* id 3, wireType 2 =*/26).string(message.alert);
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
                        case 3:
                            message.alert = reader.string();
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
                    if (message.alert != null && message.hasOwnProperty("alert"))
                        if (!$util.isString(message.alert))
                            return "alert: string expected";
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
                    if (object.alert != null)
                        message.alert = String(object.alert);
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
                        object.alert = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.postUrl != null && message.hasOwnProperty("postUrl"))
                        object.postUrl = message.postUrl;
                    if (message.alert != null && message.hasOwnProperty("alert"))
                        object.alert = message.alert;
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
    
            selfomat.FileUploadSetting = (function() {
    
                /**
                 * Properties of a FileUploadSetting.
                 * @memberof xtech.selfomat
                 * @interface IFileUploadSetting
                 * @property {string} name FileUploadSetting name
                 * @property {string} postUrl FileUploadSetting postUrl
                 * @property {string|null} [inputAccept] FileUploadSetting inputAccept
                 */
    
                /**
                 * Constructs a new FileUploadSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a FileUploadSetting.
                 * @implements IFileUploadSetting
                 * @constructor
                 * @param {xtech.selfomat.IFileUploadSetting=} [properties] Properties to set
                 */
                function FileUploadSetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * FileUploadSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @instance
                 */
                FileUploadSetting.prototype.name = "";
    
                /**
                 * FileUploadSetting postUrl.
                 * @member {string} postUrl
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @instance
                 */
                FileUploadSetting.prototype.postUrl = "";
    
                /**
                 * FileUploadSetting inputAccept.
                 * @member {string} inputAccept
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @instance
                 */
                FileUploadSetting.prototype.inputAccept = "";
    
                /**
                 * Creates a new FileUploadSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {xtech.selfomat.IFileUploadSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.FileUploadSetting} FileUploadSetting instance
                 */
                FileUploadSetting.create = function create(properties) {
                    return new FileUploadSetting(properties);
                };
    
                /**
                 * Encodes the specified FileUploadSetting message. Does not implicitly {@link xtech.selfomat.FileUploadSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {xtech.selfomat.IFileUploadSetting} message FileUploadSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                FileUploadSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.postUrl);
                    if (message.inputAccept != null && Object.hasOwnProperty.call(message, "inputAccept"))
                        writer.uint32(/* id 3, wireType 2 =*/26).string(message.inputAccept);
                    return writer;
                };
    
                /**
                 * Encodes the specified FileUploadSetting message, length delimited. Does not implicitly {@link xtech.selfomat.FileUploadSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {xtech.selfomat.IFileUploadSetting} message FileUploadSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                FileUploadSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a FileUploadSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.FileUploadSetting} FileUploadSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                FileUploadSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.FileUploadSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.postUrl = reader.string();
                            break;
                        case 3:
                            message.inputAccept = reader.string();
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
                 * Decodes a FileUploadSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.FileUploadSetting} FileUploadSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                FileUploadSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a FileUploadSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                FileUploadSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (!$util.isString(message.postUrl))
                        return "postUrl: string expected";
                    if (message.inputAccept != null && message.hasOwnProperty("inputAccept"))
                        if (!$util.isString(message.inputAccept))
                            return "inputAccept: string expected";
                    return null;
                };
    
                /**
                 * Creates a FileUploadSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.FileUploadSetting} FileUploadSetting
                 */
                FileUploadSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.FileUploadSetting)
                        return object;
                    var message = new $root.xtech.selfomat.FileUploadSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.postUrl != null)
                        message.postUrl = String(object.postUrl);
                    if (object.inputAccept != null)
                        message.inputAccept = String(object.inputAccept);
                    return message;
                };
    
                /**
                 * Creates a plain object from a FileUploadSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @static
                 * @param {xtech.selfomat.FileUploadSetting} message FileUploadSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                FileUploadSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        object.postUrl = "";
                        object.inputAccept = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.postUrl != null && message.hasOwnProperty("postUrl"))
                        object.postUrl = message.postUrl;
                    if (message.inputAccept != null && message.hasOwnProperty("inputAccept"))
                        object.inputAccept = message.inputAccept;
                    return object;
                };
    
                /**
                 * Converts this FileUploadSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.FileUploadSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                FileUploadSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return FileUploadSetting;
            })();
    
            selfomat.LinkSetting = (function() {
    
                /**
                 * Properties of a LinkSetting.
                 * @memberof xtech.selfomat
                 * @interface ILinkSetting
                 * @property {string} name LinkSetting name
                 * @property {string} url LinkSetting url
                 */
    
                /**
                 * Constructs a new LinkSetting.
                 * @memberof xtech.selfomat
                 * @classdesc Represents a LinkSetting.
                 * @implements ILinkSetting
                 * @constructor
                 * @param {xtech.selfomat.ILinkSetting=} [properties] Properties to set
                 */
                function LinkSetting(properties) {
                    if (properties)
                        for (var keys = Object.keys(properties), i = 0; i < keys.length; ++i)
                            if (properties[keys[i]] != null)
                                this[keys[i]] = properties[keys[i]];
                }
    
                /**
                 * LinkSetting name.
                 * @member {string} name
                 * @memberof xtech.selfomat.LinkSetting
                 * @instance
                 */
                LinkSetting.prototype.name = "";
    
                /**
                 * LinkSetting url.
                 * @member {string} url
                 * @memberof xtech.selfomat.LinkSetting
                 * @instance
                 */
                LinkSetting.prototype.url = "";
    
                /**
                 * Creates a new LinkSetting instance using the specified properties.
                 * @function create
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {xtech.selfomat.ILinkSetting=} [properties] Properties to set
                 * @returns {xtech.selfomat.LinkSetting} LinkSetting instance
                 */
                LinkSetting.create = function create(properties) {
                    return new LinkSetting(properties);
                };
    
                /**
                 * Encodes the specified LinkSetting message. Does not implicitly {@link xtech.selfomat.LinkSetting.verify|verify} messages.
                 * @function encode
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {xtech.selfomat.ILinkSetting} message LinkSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                LinkSetting.encode = function encode(message, writer) {
                    if (!writer)
                        writer = $Writer.create();
                    writer.uint32(/* id 1, wireType 2 =*/10).string(message.name);
                    writer.uint32(/* id 2, wireType 2 =*/18).string(message.url);
                    return writer;
                };
    
                /**
                 * Encodes the specified LinkSetting message, length delimited. Does not implicitly {@link xtech.selfomat.LinkSetting.verify|verify} messages.
                 * @function encodeDelimited
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {xtech.selfomat.ILinkSetting} message LinkSetting message or plain object to encode
                 * @param {$protobuf.Writer} [writer] Writer to encode to
                 * @returns {$protobuf.Writer} Writer
                 */
                LinkSetting.encodeDelimited = function encodeDelimited(message, writer) {
                    return this.encode(message, writer).ldelim();
                };
    
                /**
                 * Decodes a LinkSetting message from the specified reader or buffer.
                 * @function decode
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @param {number} [length] Message length if known beforehand
                 * @returns {xtech.selfomat.LinkSetting} LinkSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                LinkSetting.decode = function decode(reader, length) {
                    if (!(reader instanceof $Reader))
                        reader = $Reader.create(reader);
                    var end = length === undefined ? reader.len : reader.pos + length, message = new $root.xtech.selfomat.LinkSetting();
                    while (reader.pos < end) {
                        var tag = reader.uint32();
                        switch (tag >>> 3) {
                        case 1:
                            message.name = reader.string();
                            break;
                        case 2:
                            message.url = reader.string();
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("name"))
                        throw $util.ProtocolError("missing required 'name'", { instance: message });
                    if (!message.hasOwnProperty("url"))
                        throw $util.ProtocolError("missing required 'url'", { instance: message });
                    return message;
                };
    
                /**
                 * Decodes a LinkSetting message from the specified reader or buffer, length delimited.
                 * @function decodeDelimited
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {$protobuf.Reader|Uint8Array} reader Reader or buffer to decode from
                 * @returns {xtech.selfomat.LinkSetting} LinkSetting
                 * @throws {Error} If the payload is not a reader or valid buffer
                 * @throws {$protobuf.util.ProtocolError} If required fields are missing
                 */
                LinkSetting.decodeDelimited = function decodeDelimited(reader) {
                    if (!(reader instanceof $Reader))
                        reader = new $Reader(reader);
                    return this.decode(reader, reader.uint32());
                };
    
                /**
                 * Verifies a LinkSetting message.
                 * @function verify
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {Object.<string,*>} message Plain object to verify
                 * @returns {string|null} `null` if valid, otherwise the reason why it is not
                 */
                LinkSetting.verify = function verify(message) {
                    if (typeof message !== "object" || message === null)
                        return "object expected";
                    if (!$util.isString(message.name))
                        return "name: string expected";
                    if (!$util.isString(message.url))
                        return "url: string expected";
                    return null;
                };
    
                /**
                 * Creates a LinkSetting message from a plain object. Also converts values to their respective internal types.
                 * @function fromObject
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {Object.<string,*>} object Plain object
                 * @returns {xtech.selfomat.LinkSetting} LinkSetting
                 */
                LinkSetting.fromObject = function fromObject(object) {
                    if (object instanceof $root.xtech.selfomat.LinkSetting)
                        return object;
                    var message = new $root.xtech.selfomat.LinkSetting();
                    if (object.name != null)
                        message.name = String(object.name);
                    if (object.url != null)
                        message.url = String(object.url);
                    return message;
                };
    
                /**
                 * Creates a plain object from a LinkSetting message. Also converts values to other types if specified.
                 * @function toObject
                 * @memberof xtech.selfomat.LinkSetting
                 * @static
                 * @param {xtech.selfomat.LinkSetting} message LinkSetting
                 * @param {$protobuf.IConversionOptions} [options] Conversion options
                 * @returns {Object.<string,*>} Plain object
                 */
                LinkSetting.toObject = function toObject(message, options) {
                    if (!options)
                        options = {};
                    var object = {};
                    if (options.defaults) {
                        object.name = "";
                        object.url = "";
                    }
                    if (message.name != null && message.hasOwnProperty("name"))
                        object.name = message.name;
                    if (message.url != null && message.hasOwnProperty("url"))
                        object.url = message.url;
                    return object;
                };
    
                /**
                 * Converts this LinkSetting to JSON.
                 * @function toJSON
                 * @memberof xtech.selfomat.LinkSetting
                 * @instance
                 * @returns {Object.<string,*>} JSON object
                 */
                LinkSetting.prototype.toJSON = function toJSON() {
                    return this.constructor.toObject(this, $protobuf.util.toJSONOptions);
                };
    
                return LinkSetting;
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
                 * @property {xtech.selfomat.IListSetting} exposureCompensationTrigger CameraSettings exposureCompensationTrigger
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
                 * CameraSettings exposureCompensationTrigger.
                 * @member {xtech.selfomat.IListSetting} exposureCompensationTrigger
                 * @memberof xtech.selfomat.CameraSettings
                 * @instance
                 */
                CameraSettings.prototype.exposureCompensationTrigger = null;
    
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
                    $root.xtech.selfomat.ListSetting.encode(message.exposureCompensationTrigger, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.imageFormat, writer.uint32(/* id 6, wireType 2 =*/50).fork()).ldelim();
                    $root.xtech.selfomat.ReadOnlySetting.encode(message.cameraName, writer.uint32(/* id 7, wireType 2 =*/58).fork()).ldelim();
                    $root.xtech.selfomat.ReadOnlySetting.encode(message.lensName, writer.uint32(/* id 8, wireType 2 =*/66).fork()).ldelim();
                    if (message.focus != null && Object.hasOwnProperty.call(message, "focus"))
                        $root.xtech.selfomat.PostSetting.encode(message.focus, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
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
                            message.exposureCompensationTrigger = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 6:
                            message.imageFormat = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 7:
                            message.cameraName = $root.xtech.selfomat.ReadOnlySetting.decode(reader, reader.uint32());
                            break;
                        case 8:
                            message.lensName = $root.xtech.selfomat.ReadOnlySetting.decode(reader, reader.uint32());
                            break;
                        case 10:
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
                    if (!message.hasOwnProperty("exposureCompensationTrigger"))
                        throw $util.ProtocolError("missing required 'exposureCompensationTrigger'", { instance: message });
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
                        var error = $root.xtech.selfomat.ListSetting.verify(message.exposureCompensationTrigger);
                        if (error)
                            return "exposureCompensationTrigger." + error;
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
                    if (object.exposureCompensationTrigger != null) {
                        if (typeof object.exposureCompensationTrigger !== "object")
                            throw TypeError(".xtech.selfomat.CameraSettings.exposureCompensationTrigger: object expected");
                        message.exposureCompensationTrigger = $root.xtech.selfomat.ListSetting.fromObject(object.exposureCompensationTrigger);
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
                        object.exposureCompensationTrigger = null;
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
                    if (message.exposureCompensationTrigger != null && message.hasOwnProperty("exposureCompensationTrigger"))
                        object.exposureCompensationTrigger = $root.xtech.selfomat.ListSetting.toObject(message.exposureCompensationTrigger, options);
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
                 * @property {xtech.selfomat.IListSetting} languageChoice BoothSettings languageChoice
                 * @property {xtech.selfomat.IBoolSetting} storageEnabled BoothSettings storageEnabled
                 * @property {xtech.selfomat.IBoolSetting} printerEnabled BoothSettings printerEnabled
                 * @property {xtech.selfomat.IBoolSetting} printConfirmationEnabled BoothSettings printConfirmationEnabled
                 * @property {xtech.selfomat.IListSetting} filterChoice BoothSettings filterChoice
                 * @property {xtech.selfomat.IFloatSetting} filterGain BoothSettings filterGain
                 * @property {xtech.selfomat.IFileUploadSetting|null} [templateUpload] BoothSettings templateUpload
                 * @property {xtech.selfomat.IBoolSetting|null} [templateEnabled] BoothSettings templateEnabled
                 * @property {xtech.selfomat.IListSetting|null} [ledMode] BoothSettings ledMode
                 * @property {xtech.selfomat.IListSetting|null} [ledCount] BoothSettings ledCount
                 * @property {xtech.selfomat.IPostSetting} ledOffsetClockwise BoothSettings ledOffsetClockwise
                 * @property {xtech.selfomat.IPostSetting} ledOffsetCounterClockwise BoothSettings ledOffsetCounterClockwise
                 * @property {xtech.selfomat.IListSetting} countdownDuration BoothSettings countdownDuration
                 * @property {xtech.selfomat.IIntSetting|null} [maxLedBrightness] BoothSettings maxLedBrightness
                 * @property {xtech.selfomat.IBoolSetting} autofocusBeforeTrigger BoothSettings autofocusBeforeTrigger
                 * @property {xtech.selfomat.ILinkSetting|null} [cupsLink] BoothSettings cupsLink
                 * @property {xtech.selfomat.IPostSetting} updateMode BoothSettings updateMode
                 * @property {xtech.selfomat.IBoolSetting|null} [flashEnabled] BoothSettings flashEnabled
                 * @property {xtech.selfomat.IBoolSetting|null} [flashMode] BoothSettings flashMode
                 * @property {xtech.selfomat.IIntSetting|null} [flashDurationMicros] BoothSettings flashDurationMicros
                 * @property {xtech.selfomat.IPostSetting|null} [flashTest] BoothSettings flashTest
                 * @property {xtech.selfomat.IBoolSetting|null} [debugLogEnabled] BoothSettings debugLogEnabled
                 * @property {xtech.selfomat.IReadOnlySetting} triggerCounter BoothSettings triggerCounter
                 * @property {xtech.selfomat.IReadOnlySetting} softwareVersion BoothSettings softwareVersion
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
                 * BoothSettings languageChoice.
                 * @member {xtech.selfomat.IListSetting} languageChoice
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.languageChoice = null;
    
                /**
                 * BoothSettings storageEnabled.
                 * @member {xtech.selfomat.IBoolSetting} storageEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.storageEnabled = null;
    
                /**
                 * BoothSettings printerEnabled.
                 * @member {xtech.selfomat.IBoolSetting} printerEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.printerEnabled = null;
    
                /**
                 * BoothSettings printConfirmationEnabled.
                 * @member {xtech.selfomat.IBoolSetting} printConfirmationEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.printConfirmationEnabled = null;
    
                /**
                 * BoothSettings filterChoice.
                 * @member {xtech.selfomat.IListSetting} filterChoice
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.filterChoice = null;
    
                /**
                 * BoothSettings filterGain.
                 * @member {xtech.selfomat.IFloatSetting} filterGain
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.filterGain = null;
    
                /**
                 * BoothSettings templateUpload.
                 * @member {xtech.selfomat.IFileUploadSetting|null|undefined} templateUpload
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.templateUpload = null;
    
                /**
                 * BoothSettings templateEnabled.
                 * @member {xtech.selfomat.IBoolSetting|null|undefined} templateEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.templateEnabled = null;
    
                /**
                 * BoothSettings ledMode.
                 * @member {xtech.selfomat.IListSetting|null|undefined} ledMode
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.ledMode = null;
    
                /**
                 * BoothSettings ledCount.
                 * @member {xtech.selfomat.IListSetting|null|undefined} ledCount
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.ledCount = null;
    
                /**
                 * BoothSettings ledOffsetClockwise.
                 * @member {xtech.selfomat.IPostSetting} ledOffsetClockwise
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.ledOffsetClockwise = null;
    
                /**
                 * BoothSettings ledOffsetCounterClockwise.
                 * @member {xtech.selfomat.IPostSetting} ledOffsetCounterClockwise
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.ledOffsetCounterClockwise = null;
    
                /**
                 * BoothSettings countdownDuration.
                 * @member {xtech.selfomat.IListSetting} countdownDuration
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.countdownDuration = null;
    
                /**
                 * BoothSettings maxLedBrightness.
                 * @member {xtech.selfomat.IIntSetting|null|undefined} maxLedBrightness
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.maxLedBrightness = null;
    
                /**
                 * BoothSettings autofocusBeforeTrigger.
                 * @member {xtech.selfomat.IBoolSetting} autofocusBeforeTrigger
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.autofocusBeforeTrigger = null;
    
                /**
                 * BoothSettings cupsLink.
                 * @member {xtech.selfomat.ILinkSetting|null|undefined} cupsLink
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.cupsLink = null;
    
                /**
                 * BoothSettings updateMode.
                 * @member {xtech.selfomat.IPostSetting} updateMode
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.updateMode = null;
    
                /**
                 * BoothSettings flashEnabled.
                 * @member {xtech.selfomat.IBoolSetting|null|undefined} flashEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashEnabled = null;
    
                /**
                 * BoothSettings flashMode.
                 * @member {xtech.selfomat.IBoolSetting|null|undefined} flashMode
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashMode = null;
    
                /**
                 * BoothSettings flashDurationMicros.
                 * @member {xtech.selfomat.IIntSetting|null|undefined} flashDurationMicros
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashDurationMicros = null;
    
                /**
                 * BoothSettings flashTest.
                 * @member {xtech.selfomat.IPostSetting|null|undefined} flashTest
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.flashTest = null;
    
                /**
                 * BoothSettings debugLogEnabled.
                 * @member {xtech.selfomat.IBoolSetting|null|undefined} debugLogEnabled
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.debugLogEnabled = null;
    
                /**
                 * BoothSettings triggerCounter.
                 * @member {xtech.selfomat.IReadOnlySetting} triggerCounter
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.triggerCounter = null;
    
                /**
                 * BoothSettings softwareVersion.
                 * @member {xtech.selfomat.IReadOnlySetting} softwareVersion
                 * @memberof xtech.selfomat.BoothSettings
                 * @instance
                 */
                BoothSettings.prototype.softwareVersion = null;
    
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
                    $root.xtech.selfomat.ListSetting.encode(message.languageChoice, writer.uint32(/* id 5, wireType 2 =*/42).fork()).ldelim();
                    $root.xtech.selfomat.BoolSetting.encode(message.storageEnabled, writer.uint32(/* id 10, wireType 2 =*/82).fork()).ldelim();
                    $root.xtech.selfomat.BoolSetting.encode(message.printerEnabled, writer.uint32(/* id 11, wireType 2 =*/90).fork()).ldelim();
                    $root.xtech.selfomat.BoolSetting.encode(message.printConfirmationEnabled, writer.uint32(/* id 12, wireType 2 =*/98).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.filterChoice, writer.uint32(/* id 20, wireType 2 =*/162).fork()).ldelim();
                    $root.xtech.selfomat.FloatSetting.encode(message.filterGain, writer.uint32(/* id 21, wireType 2 =*/170).fork()).ldelim();
                    if (message.templateUpload != null && Object.hasOwnProperty.call(message, "templateUpload"))
                        $root.xtech.selfomat.FileUploadSetting.encode(message.templateUpload, writer.uint32(/* id 30, wireType 2 =*/242).fork()).ldelim();
                    if (message.templateEnabled != null && Object.hasOwnProperty.call(message, "templateEnabled"))
                        $root.xtech.selfomat.BoolSetting.encode(message.templateEnabled, writer.uint32(/* id 31, wireType 2 =*/250).fork()).ldelim();
                    if (message.ledMode != null && Object.hasOwnProperty.call(message, "ledMode"))
                        $root.xtech.selfomat.ListSetting.encode(message.ledMode, writer.uint32(/* id 40, wireType 2 =*/322).fork()).ldelim();
                    if (message.ledCount != null && Object.hasOwnProperty.call(message, "ledCount"))
                        $root.xtech.selfomat.ListSetting.encode(message.ledCount, writer.uint32(/* id 41, wireType 2 =*/330).fork()).ldelim();
                    $root.xtech.selfomat.PostSetting.encode(message.ledOffsetClockwise, writer.uint32(/* id 42, wireType 2 =*/338).fork()).ldelim();
                    $root.xtech.selfomat.PostSetting.encode(message.ledOffsetCounterClockwise, writer.uint32(/* id 43, wireType 2 =*/346).fork()).ldelim();
                    $root.xtech.selfomat.ListSetting.encode(message.countdownDuration, writer.uint32(/* id 44, wireType 2 =*/354).fork()).ldelim();
                    if (message.maxLedBrightness != null && Object.hasOwnProperty.call(message, "maxLedBrightness"))
                        $root.xtech.selfomat.IntSetting.encode(message.maxLedBrightness, writer.uint32(/* id 45, wireType 2 =*/362).fork()).ldelim();
                    $root.xtech.selfomat.BoolSetting.encode(message.autofocusBeforeTrigger, writer.uint32(/* id 46, wireType 2 =*/370).fork()).ldelim();
                    if (message.cupsLink != null && Object.hasOwnProperty.call(message, "cupsLink"))
                        $root.xtech.selfomat.LinkSetting.encode(message.cupsLink, writer.uint32(/* id 50, wireType 2 =*/402).fork()).ldelim();
                    $root.xtech.selfomat.PostSetting.encode(message.updateMode, writer.uint32(/* id 51, wireType 2 =*/410).fork()).ldelim();
                    if (message.flashEnabled != null && Object.hasOwnProperty.call(message, "flashEnabled"))
                        $root.xtech.selfomat.BoolSetting.encode(message.flashEnabled, writer.uint32(/* id 200, wireType 2 =*/1602).fork()).ldelim();
                    if (message.flashMode != null && Object.hasOwnProperty.call(message, "flashMode"))
                        $root.xtech.selfomat.BoolSetting.encode(message.flashMode, writer.uint32(/* id 201, wireType 2 =*/1610).fork()).ldelim();
                    if (message.flashDurationMicros != null && Object.hasOwnProperty.call(message, "flashDurationMicros"))
                        $root.xtech.selfomat.IntSetting.encode(message.flashDurationMicros, writer.uint32(/* id 202, wireType 2 =*/1618).fork()).ldelim();
                    if (message.flashTest != null && Object.hasOwnProperty.call(message, "flashTest"))
                        $root.xtech.selfomat.PostSetting.encode(message.flashTest, writer.uint32(/* id 203, wireType 2 =*/1626).fork()).ldelim();
                    if (message.debugLogEnabled != null && Object.hasOwnProperty.call(message, "debugLogEnabled"))
                        $root.xtech.selfomat.BoolSetting.encode(message.debugLogEnabled, writer.uint32(/* id 240, wireType 2 =*/1922).fork()).ldelim();
                    $root.xtech.selfomat.ReadOnlySetting.encode(message.triggerCounter, writer.uint32(/* id 250, wireType 2 =*/2002).fork()).ldelim();
                    $root.xtech.selfomat.ReadOnlySetting.encode(message.softwareVersion, writer.uint32(/* id 255, wireType 2 =*/2042).fork()).ldelim();
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
                        case 5:
                            message.languageChoice = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 10:
                            message.storageEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 11:
                            message.printerEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 12:
                            message.printConfirmationEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 20:
                            message.filterChoice = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 21:
                            message.filterGain = $root.xtech.selfomat.FloatSetting.decode(reader, reader.uint32());
                            break;
                        case 30:
                            message.templateUpload = $root.xtech.selfomat.FileUploadSetting.decode(reader, reader.uint32());
                            break;
                        case 31:
                            message.templateEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 40:
                            message.ledMode = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 41:
                            message.ledCount = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 42:
                            message.ledOffsetClockwise = $root.xtech.selfomat.PostSetting.decode(reader, reader.uint32());
                            break;
                        case 43:
                            message.ledOffsetCounterClockwise = $root.xtech.selfomat.PostSetting.decode(reader, reader.uint32());
                            break;
                        case 44:
                            message.countdownDuration = $root.xtech.selfomat.ListSetting.decode(reader, reader.uint32());
                            break;
                        case 45:
                            message.maxLedBrightness = $root.xtech.selfomat.IntSetting.decode(reader, reader.uint32());
                            break;
                        case 46:
                            message.autofocusBeforeTrigger = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 50:
                            message.cupsLink = $root.xtech.selfomat.LinkSetting.decode(reader, reader.uint32());
                            break;
                        case 51:
                            message.updateMode = $root.xtech.selfomat.PostSetting.decode(reader, reader.uint32());
                            break;
                        case 200:
                            message.flashEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 201:
                            message.flashMode = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 202:
                            message.flashDurationMicros = $root.xtech.selfomat.IntSetting.decode(reader, reader.uint32());
                            break;
                        case 203:
                            message.flashTest = $root.xtech.selfomat.PostSetting.decode(reader, reader.uint32());
                            break;
                        case 240:
                            message.debugLogEnabled = $root.xtech.selfomat.BoolSetting.decode(reader, reader.uint32());
                            break;
                        case 250:
                            message.triggerCounter = $root.xtech.selfomat.ReadOnlySetting.decode(reader, reader.uint32());
                            break;
                        case 255:
                            message.softwareVersion = $root.xtech.selfomat.ReadOnlySetting.decode(reader, reader.uint32());
                            break;
                        default:
                            reader.skipType(tag & 7);
                            break;
                        }
                    }
                    if (!message.hasOwnProperty("languageChoice"))
                        throw $util.ProtocolError("missing required 'languageChoice'", { instance: message });
                    if (!message.hasOwnProperty("storageEnabled"))
                        throw $util.ProtocolError("missing required 'storageEnabled'", { instance: message });
                    if (!message.hasOwnProperty("printerEnabled"))
                        throw $util.ProtocolError("missing required 'printerEnabled'", { instance: message });
                    if (!message.hasOwnProperty("printConfirmationEnabled"))
                        throw $util.ProtocolError("missing required 'printConfirmationEnabled'", { instance: message });
                    if (!message.hasOwnProperty("filterChoice"))
                        throw $util.ProtocolError("missing required 'filterChoice'", { instance: message });
                    if (!message.hasOwnProperty("filterGain"))
                        throw $util.ProtocolError("missing required 'filterGain'", { instance: message });
                    if (!message.hasOwnProperty("ledOffsetClockwise"))
                        throw $util.ProtocolError("missing required 'ledOffsetClockwise'", { instance: message });
                    if (!message.hasOwnProperty("ledOffsetCounterClockwise"))
                        throw $util.ProtocolError("missing required 'ledOffsetCounterClockwise'", { instance: message });
                    if (!message.hasOwnProperty("countdownDuration"))
                        throw $util.ProtocolError("missing required 'countdownDuration'", { instance: message });
                    if (!message.hasOwnProperty("autofocusBeforeTrigger"))
                        throw $util.ProtocolError("missing required 'autofocusBeforeTrigger'", { instance: message });
                    if (!message.hasOwnProperty("updateMode"))
                        throw $util.ProtocolError("missing required 'updateMode'", { instance: message });
                    if (!message.hasOwnProperty("triggerCounter"))
                        throw $util.ProtocolError("missing required 'triggerCounter'", { instance: message });
                    if (!message.hasOwnProperty("softwareVersion"))
                        throw $util.ProtocolError("missing required 'softwareVersion'", { instance: message });
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
                        var error = $root.xtech.selfomat.ListSetting.verify(message.languageChoice);
                        if (error)
                            return "languageChoice." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.storageEnabled);
                        if (error)
                            return "storageEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.printerEnabled);
                        if (error)
                            return "printerEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.printConfirmationEnabled);
                        if (error)
                            return "printConfirmationEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.filterChoice);
                        if (error)
                            return "filterChoice." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.FloatSetting.verify(message.filterGain);
                        if (error)
                            return "filterGain." + error;
                    }
                    if (message.templateUpload != null && message.hasOwnProperty("templateUpload")) {
                        var error = $root.xtech.selfomat.FileUploadSetting.verify(message.templateUpload);
                        if (error)
                            return "templateUpload." + error;
                    }
                    if (message.templateEnabled != null && message.hasOwnProperty("templateEnabled")) {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.templateEnabled);
                        if (error)
                            return "templateEnabled." + error;
                    }
                    if (message.ledMode != null && message.hasOwnProperty("ledMode")) {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.ledMode);
                        if (error)
                            return "ledMode." + error;
                    }
                    if (message.ledCount != null && message.hasOwnProperty("ledCount")) {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.ledCount);
                        if (error)
                            return "ledCount." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.PostSetting.verify(message.ledOffsetClockwise);
                        if (error)
                            return "ledOffsetClockwise." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.PostSetting.verify(message.ledOffsetCounterClockwise);
                        if (error)
                            return "ledOffsetCounterClockwise." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ListSetting.verify(message.countdownDuration);
                        if (error)
                            return "countdownDuration." + error;
                    }
                    if (message.maxLedBrightness != null && message.hasOwnProperty("maxLedBrightness")) {
                        var error = $root.xtech.selfomat.IntSetting.verify(message.maxLedBrightness);
                        if (error)
                            return "maxLedBrightness." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.autofocusBeforeTrigger);
                        if (error)
                            return "autofocusBeforeTrigger." + error;
                    }
                    if (message.cupsLink != null && message.hasOwnProperty("cupsLink")) {
                        var error = $root.xtech.selfomat.LinkSetting.verify(message.cupsLink);
                        if (error)
                            return "cupsLink." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.PostSetting.verify(message.updateMode);
                        if (error)
                            return "updateMode." + error;
                    }
                    if (message.flashEnabled != null && message.hasOwnProperty("flashEnabled")) {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.flashEnabled);
                        if (error)
                            return "flashEnabled." + error;
                    }
                    if (message.flashMode != null && message.hasOwnProperty("flashMode")) {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.flashMode);
                        if (error)
                            return "flashMode." + error;
                    }
                    if (message.flashDurationMicros != null && message.hasOwnProperty("flashDurationMicros")) {
                        var error = $root.xtech.selfomat.IntSetting.verify(message.flashDurationMicros);
                        if (error)
                            return "flashDurationMicros." + error;
                    }
                    if (message.flashTest != null && message.hasOwnProperty("flashTest")) {
                        var error = $root.xtech.selfomat.PostSetting.verify(message.flashTest);
                        if (error)
                            return "flashTest." + error;
                    }
                    if (message.debugLogEnabled != null && message.hasOwnProperty("debugLogEnabled")) {
                        var error = $root.xtech.selfomat.BoolSetting.verify(message.debugLogEnabled);
                        if (error)
                            return "debugLogEnabled." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ReadOnlySetting.verify(message.triggerCounter);
                        if (error)
                            return "triggerCounter." + error;
                    }
                    {
                        var error = $root.xtech.selfomat.ReadOnlySetting.verify(message.softwareVersion);
                        if (error)
                            return "softwareVersion." + error;
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
                    if (object.languageChoice != null) {
                        if (typeof object.languageChoice !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.languageChoice: object expected");
                        message.languageChoice = $root.xtech.selfomat.ListSetting.fromObject(object.languageChoice);
                    }
                    if (object.storageEnabled != null) {
                        if (typeof object.storageEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.storageEnabled: object expected");
                        message.storageEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.storageEnabled);
                    }
                    if (object.printerEnabled != null) {
                        if (typeof object.printerEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.printerEnabled: object expected");
                        message.printerEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.printerEnabled);
                    }
                    if (object.printConfirmationEnabled != null) {
                        if (typeof object.printConfirmationEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.printConfirmationEnabled: object expected");
                        message.printConfirmationEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.printConfirmationEnabled);
                    }
                    if (object.filterChoice != null) {
                        if (typeof object.filterChoice !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.filterChoice: object expected");
                        message.filterChoice = $root.xtech.selfomat.ListSetting.fromObject(object.filterChoice);
                    }
                    if (object.filterGain != null) {
                        if (typeof object.filterGain !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.filterGain: object expected");
                        message.filterGain = $root.xtech.selfomat.FloatSetting.fromObject(object.filterGain);
                    }
                    if (object.templateUpload != null) {
                        if (typeof object.templateUpload !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.templateUpload: object expected");
                        message.templateUpload = $root.xtech.selfomat.FileUploadSetting.fromObject(object.templateUpload);
                    }
                    if (object.templateEnabled != null) {
                        if (typeof object.templateEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.templateEnabled: object expected");
                        message.templateEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.templateEnabled);
                    }
                    if (object.ledMode != null) {
                        if (typeof object.ledMode !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.ledMode: object expected");
                        message.ledMode = $root.xtech.selfomat.ListSetting.fromObject(object.ledMode);
                    }
                    if (object.ledCount != null) {
                        if (typeof object.ledCount !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.ledCount: object expected");
                        message.ledCount = $root.xtech.selfomat.ListSetting.fromObject(object.ledCount);
                    }
                    if (object.ledOffsetClockwise != null) {
                        if (typeof object.ledOffsetClockwise !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.ledOffsetClockwise: object expected");
                        message.ledOffsetClockwise = $root.xtech.selfomat.PostSetting.fromObject(object.ledOffsetClockwise);
                    }
                    if (object.ledOffsetCounterClockwise != null) {
                        if (typeof object.ledOffsetCounterClockwise !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.ledOffsetCounterClockwise: object expected");
                        message.ledOffsetCounterClockwise = $root.xtech.selfomat.PostSetting.fromObject(object.ledOffsetCounterClockwise);
                    }
                    if (object.countdownDuration != null) {
                        if (typeof object.countdownDuration !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.countdownDuration: object expected");
                        message.countdownDuration = $root.xtech.selfomat.ListSetting.fromObject(object.countdownDuration);
                    }
                    if (object.maxLedBrightness != null) {
                        if (typeof object.maxLedBrightness !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.maxLedBrightness: object expected");
                        message.maxLedBrightness = $root.xtech.selfomat.IntSetting.fromObject(object.maxLedBrightness);
                    }
                    if (object.autofocusBeforeTrigger != null) {
                        if (typeof object.autofocusBeforeTrigger !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.autofocusBeforeTrigger: object expected");
                        message.autofocusBeforeTrigger = $root.xtech.selfomat.BoolSetting.fromObject(object.autofocusBeforeTrigger);
                    }
                    if (object.cupsLink != null) {
                        if (typeof object.cupsLink !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.cupsLink: object expected");
                        message.cupsLink = $root.xtech.selfomat.LinkSetting.fromObject(object.cupsLink);
                    }
                    if (object.updateMode != null) {
                        if (typeof object.updateMode !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.updateMode: object expected");
                        message.updateMode = $root.xtech.selfomat.PostSetting.fromObject(object.updateMode);
                    }
                    if (object.flashEnabled != null) {
                        if (typeof object.flashEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashEnabled: object expected");
                        message.flashEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.flashEnabled);
                    }
                    if (object.flashMode != null) {
                        if (typeof object.flashMode !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashMode: object expected");
                        message.flashMode = $root.xtech.selfomat.BoolSetting.fromObject(object.flashMode);
                    }
                    if (object.flashDurationMicros != null) {
                        if (typeof object.flashDurationMicros !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashDurationMicros: object expected");
                        message.flashDurationMicros = $root.xtech.selfomat.IntSetting.fromObject(object.flashDurationMicros);
                    }
                    if (object.flashTest != null) {
                        if (typeof object.flashTest !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.flashTest: object expected");
                        message.flashTest = $root.xtech.selfomat.PostSetting.fromObject(object.flashTest);
                    }
                    if (object.debugLogEnabled != null) {
                        if (typeof object.debugLogEnabled !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.debugLogEnabled: object expected");
                        message.debugLogEnabled = $root.xtech.selfomat.BoolSetting.fromObject(object.debugLogEnabled);
                    }
                    if (object.triggerCounter != null) {
                        if (typeof object.triggerCounter !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.triggerCounter: object expected");
                        message.triggerCounter = $root.xtech.selfomat.ReadOnlySetting.fromObject(object.triggerCounter);
                    }
                    if (object.softwareVersion != null) {
                        if (typeof object.softwareVersion !== "object")
                            throw TypeError(".xtech.selfomat.BoothSettings.softwareVersion: object expected");
                        message.softwareVersion = $root.xtech.selfomat.ReadOnlySetting.fromObject(object.softwareVersion);
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
                        object.languageChoice = null;
                        object.storageEnabled = null;
                        object.printerEnabled = null;
                        object.printConfirmationEnabled = null;
                        object.filterChoice = null;
                        object.filterGain = null;
                        object.templateUpload = null;
                        object.templateEnabled = null;
                        object.ledMode = null;
                        object.ledCount = null;
                        object.ledOffsetClockwise = null;
                        object.ledOffsetCounterClockwise = null;
                        object.countdownDuration = null;
                        object.maxLedBrightness = null;
                        object.autofocusBeforeTrigger = null;
                        object.cupsLink = null;
                        object.updateMode = null;
                        object.flashEnabled = null;
                        object.flashMode = null;
                        object.flashDurationMicros = null;
                        object.flashTest = null;
                        object.debugLogEnabled = null;
                        object.triggerCounter = null;
                        object.softwareVersion = null;
                    }
                    if (message.languageChoice != null && message.hasOwnProperty("languageChoice"))
                        object.languageChoice = $root.xtech.selfomat.ListSetting.toObject(message.languageChoice, options);
                    if (message.storageEnabled != null && message.hasOwnProperty("storageEnabled"))
                        object.storageEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.storageEnabled, options);
                    if (message.printerEnabled != null && message.hasOwnProperty("printerEnabled"))
                        object.printerEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.printerEnabled, options);
                    if (message.printConfirmationEnabled != null && message.hasOwnProperty("printConfirmationEnabled"))
                        object.printConfirmationEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.printConfirmationEnabled, options);
                    if (message.filterChoice != null && message.hasOwnProperty("filterChoice"))
                        object.filterChoice = $root.xtech.selfomat.ListSetting.toObject(message.filterChoice, options);
                    if (message.filterGain != null && message.hasOwnProperty("filterGain"))
                        object.filterGain = $root.xtech.selfomat.FloatSetting.toObject(message.filterGain, options);
                    if (message.templateUpload != null && message.hasOwnProperty("templateUpload"))
                        object.templateUpload = $root.xtech.selfomat.FileUploadSetting.toObject(message.templateUpload, options);
                    if (message.templateEnabled != null && message.hasOwnProperty("templateEnabled"))
                        object.templateEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.templateEnabled, options);
                    if (message.ledMode != null && message.hasOwnProperty("ledMode"))
                        object.ledMode = $root.xtech.selfomat.ListSetting.toObject(message.ledMode, options);
                    if (message.ledCount != null && message.hasOwnProperty("ledCount"))
                        object.ledCount = $root.xtech.selfomat.ListSetting.toObject(message.ledCount, options);
                    if (message.ledOffsetClockwise != null && message.hasOwnProperty("ledOffsetClockwise"))
                        object.ledOffsetClockwise = $root.xtech.selfomat.PostSetting.toObject(message.ledOffsetClockwise, options);
                    if (message.ledOffsetCounterClockwise != null && message.hasOwnProperty("ledOffsetCounterClockwise"))
                        object.ledOffsetCounterClockwise = $root.xtech.selfomat.PostSetting.toObject(message.ledOffsetCounterClockwise, options);
                    if (message.countdownDuration != null && message.hasOwnProperty("countdownDuration"))
                        object.countdownDuration = $root.xtech.selfomat.ListSetting.toObject(message.countdownDuration, options);
                    if (message.maxLedBrightness != null && message.hasOwnProperty("maxLedBrightness"))
                        object.maxLedBrightness = $root.xtech.selfomat.IntSetting.toObject(message.maxLedBrightness, options);
                    if (message.autofocusBeforeTrigger != null && message.hasOwnProperty("autofocusBeforeTrigger"))
                        object.autofocusBeforeTrigger = $root.xtech.selfomat.BoolSetting.toObject(message.autofocusBeforeTrigger, options);
                    if (message.cupsLink != null && message.hasOwnProperty("cupsLink"))
                        object.cupsLink = $root.xtech.selfomat.LinkSetting.toObject(message.cupsLink, options);
                    if (message.updateMode != null && message.hasOwnProperty("updateMode"))
                        object.updateMode = $root.xtech.selfomat.PostSetting.toObject(message.updateMode, options);
                    if (message.flashEnabled != null && message.hasOwnProperty("flashEnabled"))
                        object.flashEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.flashEnabled, options);
                    if (message.flashMode != null && message.hasOwnProperty("flashMode"))
                        object.flashMode = $root.xtech.selfomat.BoolSetting.toObject(message.flashMode, options);
                    if (message.flashDurationMicros != null && message.hasOwnProperty("flashDurationMicros"))
                        object.flashDurationMicros = $root.xtech.selfomat.IntSetting.toObject(message.flashDurationMicros, options);
                    if (message.flashTest != null && message.hasOwnProperty("flashTest"))
                        object.flashTest = $root.xtech.selfomat.PostSetting.toObject(message.flashTest, options);
                    if (message.debugLogEnabled != null && message.hasOwnProperty("debugLogEnabled"))
                        object.debugLogEnabled = $root.xtech.selfomat.BoolSetting.toObject(message.debugLogEnabled, options);
                    if (message.triggerCounter != null && message.hasOwnProperty("triggerCounter"))
                        object.triggerCounter = $root.xtech.selfomat.ReadOnlySetting.toObject(message.triggerCounter, options);
                    if (message.softwareVersion != null && message.hasOwnProperty("softwareVersion"))
                        object.softwareVersion = $root.xtech.selfomat.ReadOnlySetting.toObject(message.softwareVersion, options);
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
