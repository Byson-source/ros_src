// Auto-generated. Do not edit!

// (in-package cpp.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class StringArray {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.strings = null;
    }
    else {
      if (initObj.hasOwnProperty('strings')) {
        this.strings = initObj.strings
      }
      else {
        this.strings = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type StringArray
    // Serialize message field [strings]
    bufferOffset = _arraySerializer.string(obj.strings, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type StringArray
    let len;
    let data = new StringArray(null);
    // Deserialize message field [strings]
    data.strings = _arrayDeserializer.string(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    object.strings.forEach((val) => {
      length += 4 + _getByteLength(val);
    });
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'cpp/StringArray';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '51789d20146e565223d0963361aecda1';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string[] strings
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new StringArray(null);
    if (msg.strings !== undefined) {
      resolved.strings = msg.strings;
    }
    else {
      resolved.strings = []
    }

    return resolved;
    }
};

module.exports = StringArray;
