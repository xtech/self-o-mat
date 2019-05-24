import * as $protobuf from "protobufjs";
export namespace xtech {

    namespace selfomat {

        interface IStatus {
            currentState: xtech.selfomat.Status.State;
        }

        class Status implements IStatus {
            constructor(properties?: xtech.selfomat.IStatus);
            public currentState: xtech.selfomat.Status.State;
            public static create(properties?: xtech.selfomat.IStatus): xtech.selfomat.Status;
            public static encode(message: xtech.selfomat.IStatus, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IStatus, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.Status;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.Status;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.Status;
            public static toObject(message: xtech.selfomat.Status, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        namespace Status {

            enum State {
                INITIALIZING = 0,
                RUNNING = 1,
                ERROR = 2
            }
        }

        interface IIntUpdate {
            value: (number|Long);
        }

        class IntUpdate implements IIntUpdate {
            constructor(properties?: xtech.selfomat.IIntUpdate);
            public value: (number|Long);
            public static create(properties?: xtech.selfomat.IIntUpdate): xtech.selfomat.IntUpdate;
            public static encode(message: xtech.selfomat.IIntUpdate, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IIntUpdate, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.IntUpdate;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.IntUpdate;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.IntUpdate;
            public static toObject(message: xtech.selfomat.IntUpdate, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IBoolUpdate {
            value: boolean;
        }

        class BoolUpdate implements IBoolUpdate {
            constructor(properties?: xtech.selfomat.IBoolUpdate);
            public value: boolean;
            public static create(properties?: xtech.selfomat.IBoolUpdate): xtech.selfomat.BoolUpdate;
            public static encode(message: xtech.selfomat.IBoolUpdate, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IBoolUpdate, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.BoolUpdate;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.BoolUpdate;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.BoolUpdate;
            public static toObject(message: xtech.selfomat.BoolUpdate, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IFloatUpdate {
            value: number;
        }

        class FloatUpdate implements IFloatUpdate {
            constructor(properties?: xtech.selfomat.IFloatUpdate);
            public value: number;
            public static create(properties?: xtech.selfomat.IFloatUpdate): xtech.selfomat.FloatUpdate;
            public static encode(message: xtech.selfomat.IFloatUpdate, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IFloatUpdate, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.FloatUpdate;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.FloatUpdate;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.FloatUpdate;
            public static toObject(message: xtech.selfomat.FloatUpdate, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IReadOnlySetting {
            name: string;
            value: string;
        }

        class ReadOnlySetting implements IReadOnlySetting {
            constructor(properties?: xtech.selfomat.IReadOnlySetting);
            public name: string;
            public value: string;
            public static create(properties?: xtech.selfomat.IReadOnlySetting): xtech.selfomat.ReadOnlySetting;
            public static encode(message: xtech.selfomat.IReadOnlySetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IReadOnlySetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.ReadOnlySetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.ReadOnlySetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.ReadOnlySetting;
            public static toObject(message: xtech.selfomat.ReadOnlySetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IPostSetting {
            name: string;
            postUrl: string;
            alert?: (string|null);
        }

        class PostSetting implements IPostSetting {
            constructor(properties?: xtech.selfomat.IPostSetting);
            public name: string;
            public postUrl: string;
            public alert: string;
            public static create(properties?: xtech.selfomat.IPostSetting): xtech.selfomat.PostSetting;
            public static encode(message: xtech.selfomat.IPostSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IPostSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.PostSetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.PostSetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.PostSetting;
            public static toObject(message: xtech.selfomat.PostSetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IFileUploadSetting {
            name: string;
            postUrl: string;
            inputAccept?: (string|null);
        }

        class FileUploadSetting implements IFileUploadSetting {
            constructor(properties?: xtech.selfomat.IFileUploadSetting);
            public name: string;
            public postUrl: string;
            public inputAccept: string;
            public static create(properties?: xtech.selfomat.IFileUploadSetting): xtech.selfomat.FileUploadSetting;
            public static encode(message: xtech.selfomat.IFileUploadSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IFileUploadSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.FileUploadSetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.FileUploadSetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.FileUploadSetting;
            public static toObject(message: xtech.selfomat.FileUploadSetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IListSetting {
            name: string;
            currentIndex: number;
            values?: (string[]|null);
            updateUrl: string;
        }

        class ListSetting implements IListSetting {
            constructor(properties?: xtech.selfomat.IListSetting);
            public name: string;
            public currentIndex: number;
            public values: string[];
            public updateUrl: string;
            public static create(properties?: xtech.selfomat.IListSetting): xtech.selfomat.ListSetting;
            public static encode(message: xtech.selfomat.IListSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IListSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.ListSetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.ListSetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.ListSetting;
            public static toObject(message: xtech.selfomat.ListSetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IBoolSetting {
            name: string;
            currentValue: boolean;
            updateUrl: string;
        }

        class BoolSetting implements IBoolSetting {
            constructor(properties?: xtech.selfomat.IBoolSetting);
            public name: string;
            public currentValue: boolean;
            public updateUrl: string;
            public static create(properties?: xtech.selfomat.IBoolSetting): xtech.selfomat.BoolSetting;
            public static encode(message: xtech.selfomat.IBoolSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IBoolSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.BoolSetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.BoolSetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.BoolSetting;
            public static toObject(message: xtech.selfomat.BoolSetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IFloatSetting {
            name: string;
            currentValue: number;
            minValue: number;
            maxValue: number;
            updateUrl: string;
        }

        class FloatSetting implements IFloatSetting {
            constructor(properties?: xtech.selfomat.IFloatSetting);
            public name: string;
            public currentValue: number;
            public minValue: number;
            public maxValue: number;
            public updateUrl: string;
            public static create(properties?: xtech.selfomat.IFloatSetting): xtech.selfomat.FloatSetting;
            public static encode(message: xtech.selfomat.IFloatSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IFloatSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.FloatSetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.FloatSetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.FloatSetting;
            public static toObject(message: xtech.selfomat.FloatSetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IIntSetting {
            name: string;
            currentValue: (number|Long);
            minValue: (number|Long);
            maxValue: (number|Long);
            updateUrl: string;
        }

        class IntSetting implements IIntSetting {
            constructor(properties?: xtech.selfomat.IIntSetting);
            public name: string;
            public currentValue: (number|Long);
            public minValue: (number|Long);
            public maxValue: (number|Long);
            public updateUrl: string;
            public static create(properties?: xtech.selfomat.IIntSetting): xtech.selfomat.IntSetting;
            public static encode(message: xtech.selfomat.IIntSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IIntSetting, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.IntSetting;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.IntSetting;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.IntSetting;
            public static toObject(message: xtech.selfomat.IntSetting, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface ICameraSettings {
            iso: xtech.selfomat.IListSetting;
            shutterSpeed: xtech.selfomat.IListSetting;
            aperture: xtech.selfomat.IListSetting;
            exposureCompensation: xtech.selfomat.IListSetting;
            exposureCompensationTrigger: xtech.selfomat.IListSetting;
            imageFormat: xtech.selfomat.IListSetting;
            cameraName: xtech.selfomat.IReadOnlySetting;
            lensName: xtech.selfomat.IReadOnlySetting;
            focus?: (xtech.selfomat.IPostSetting|null);
        }

        class CameraSettings implements ICameraSettings {
            constructor(properties?: xtech.selfomat.ICameraSettings);
            public iso: xtech.selfomat.IListSetting;
            public shutterSpeed: xtech.selfomat.IListSetting;
            public aperture: xtech.selfomat.IListSetting;
            public exposureCompensation: xtech.selfomat.IListSetting;
            public exposureCompensationTrigger: xtech.selfomat.IListSetting;
            public imageFormat: xtech.selfomat.IListSetting;
            public cameraName: xtech.selfomat.IReadOnlySetting;
            public lensName: xtech.selfomat.IReadOnlySetting;
            public focus?: (xtech.selfomat.IPostSetting|null);
            public static create(properties?: xtech.selfomat.ICameraSettings): xtech.selfomat.CameraSettings;
            public static encode(message: xtech.selfomat.ICameraSettings, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.ICameraSettings, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.CameraSettings;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.CameraSettings;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.CameraSettings;
            public static toObject(message: xtech.selfomat.CameraSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }

        interface IBoothSettings {
            storageEnabled: xtech.selfomat.IBoolSetting;
            printerEnabled: xtech.selfomat.IBoolSetting;
            flashEnabled: xtech.selfomat.IBoolSetting;
            flashDurationMicros: xtech.selfomat.IIntSetting;
            flashTest: xtech.selfomat.IPostSetting;
            templateUpload?: (xtech.selfomat.IFileUploadSetting|null);
            templateEnabled?: (xtech.selfomat.IBoolSetting|null);
            ledMode?: (xtech.selfomat.IListSetting|null);
            ledCount?: (xtech.selfomat.IListSetting|null);
            ledOffset: xtech.selfomat.IListSetting;
            countdownDuration: xtech.selfomat.IListSetting;
            updateMode: xtech.selfomat.IPostSetting;
        }

        class BoothSettings implements IBoothSettings {
            constructor(properties?: xtech.selfomat.IBoothSettings);
            public storageEnabled: xtech.selfomat.IBoolSetting;
            public printerEnabled: xtech.selfomat.IBoolSetting;
            public flashEnabled: xtech.selfomat.IBoolSetting;
            public flashDurationMicros: xtech.selfomat.IIntSetting;
            public flashTest: xtech.selfomat.IPostSetting;
            public templateUpload?: (xtech.selfomat.IFileUploadSetting|null);
            public templateEnabled?: (xtech.selfomat.IBoolSetting|null);
            public ledMode?: (xtech.selfomat.IListSetting|null);
            public ledCount?: (xtech.selfomat.IListSetting|null);
            public ledOffset: xtech.selfomat.IListSetting;
            public countdownDuration: xtech.selfomat.IListSetting;
            public updateMode: xtech.selfomat.IPostSetting;
            public static create(properties?: xtech.selfomat.IBoothSettings): xtech.selfomat.BoothSettings;
            public static encode(message: xtech.selfomat.IBoothSettings, writer?: $protobuf.Writer): $protobuf.Writer;
            public static encodeDelimited(message: xtech.selfomat.IBoothSettings, writer?: $protobuf.Writer): $protobuf.Writer;
            public static decode(reader: ($protobuf.Reader|Uint8Array), length?: number): xtech.selfomat.BoothSettings;
            public static decodeDelimited(reader: ($protobuf.Reader|Uint8Array)): xtech.selfomat.BoothSettings;
            public static verify(message: { [k: string]: any }): (string|null);
            public static fromObject(object: { [k: string]: any }): xtech.selfomat.BoothSettings;
            public static toObject(message: xtech.selfomat.BoothSettings, options?: $protobuf.IConversionOptions): { [k: string]: any };
            public toJSON(): { [k: string]: any };
        }
    }
}
