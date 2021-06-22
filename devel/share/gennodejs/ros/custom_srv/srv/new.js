// Auto-generated. Do not edit!

// (in-package custom_srv.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class newRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.name1 = null;
      this.name2 = null;
    }
    else {
      if (initObj.hasOwnProperty('name1')) {
        this.name1 = initObj.name1
      }
      else {
        this.name1 = '';
      }
      if (initObj.hasOwnProperty('name2')) {
        this.name2 = initObj.name2
      }
      else {
        this.name2 = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type newRequest
    // Serialize message field [name1]
    bufferOffset = _serializer.string(obj.name1, buffer, bufferOffset);
    // Serialize message field [name2]
    bufferOffset = _serializer.string(obj.name2, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type newRequest
    let len;
    let data = new newRequest(null);
    // Deserialize message field [name1]
    data.name1 = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [name2]
    data.name2 = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.name1);
    length += _getByteLength(object.name2);
    return length + 8;
  }

  static datatype() {
    // Returns string type for a service object
    return 'custom_srv/newRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'a212a9dcf203631b136bbebfc520ad3c';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string name1
    string name2
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new newRequest(null);
    if (msg.name1 !== undefined) {
      resolved.name1 = msg.name1;
    }
    else {
      resolved.name1 = ''
    }

    if (msg.name2 !== undefined) {
      resolved.name2 = msg.name2;
    }
    else {
      resolved.name2 = ''
    }

    return resolved;
    }
};

class newResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.fullname = null;
    }
    else {
      if (initObj.hasOwnProperty('fullname')) {
        this.fullname = initObj.fullname
      }
      else {
        this.fullname = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type newResponse
    // Serialize message field [fullname]
    bufferOffset = _serializer.string(obj.fullname, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type newResponse
    let len;
    let data = new newResponse(null);
    // Deserialize message field [fullname]
    data.fullname = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += _getByteLength(object.fullname);
    return length + 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'custom_srv/newResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4b3ec2b8015eb1b3aac2c2800e66d332';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string fullname
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new newResponse(null);
    if (msg.fullname !== undefined) {
      resolved.fullname = msg.fullname;
    }
    else {
      resolved.fullname = ''
    }

    return resolved;
    }
};

module.exports = {
  Request: newRequest,
  Response: newResponse,
  md5sum() { return 'bedb2791e8aadec249d6cb1fea325ad5'; },
  datatype() { return 'custom_srv/new'; }
};
